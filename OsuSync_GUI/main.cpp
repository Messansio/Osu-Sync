#include <OsuPathUtility.h>
#include <CompressionUtility.h>
#include <raylib.h>
#include <stdexcept>

#include <iostream>

bool selectIfSync(std::atomic<bool> &isRunning) {
    bool isSyncing = false;

    bool localIsRunning = isRunning.load();

    while (!IsKeyPressed(KEY_ENTER) && localIsRunning) {
        std::string message = "Current mode: ";

        if (IsKeyPressed(KEY_SPACE)) isSyncing = !isSyncing;

        if (isSyncing) message.append("copy files (Press space to switch to sync)");
        else message.append("sync files (Press space to switch to copy)");

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(message.c_str(), 190, 200, 20, LIGHTGRAY);
        EndDrawing();
        if(WindowShouldClose()) localIsRunning = false; 
    }
    isRunning.store(localIsRunning);
    return isSyncing;
}

void waitNoLock(const int &maxSec) {
    const int maxFrames = maxSec * 60;
    int countedFrames = 0;
    while (countedFrames < maxFrames) {
        countedFrames++;
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Remember to Press enter to confirm", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }
}

std::pair<bool,bool> selectClient(std::atomic<bool> &isRunning) {
    std::pair<bool,bool> whatClient = std::make_pair(false, true);
    short int lastSelected = 0;

    bool localIsRunning = isRunning.load();

    while (!IsKeyPressed(KEY_ENTER) && localIsRunning) {
        std::cout << "work\n";
        std::string message;

        if (IsKeyPressed(KEY_SPACE)) lastSelected = (lastSelected + 1) % 3;

        if (lastSelected == 0) {
            whatClient.first = true;
            whatClient.second = true;
        } else if (lastSelected == 1) {
            whatClient.first = true;
            whatClient.second = false;
        } else if (lastSelected == 2) {
            whatClient.first = false;
            whatClient.second = true;
        }

        if (whatClient.first && whatClient.second) message = "stable and lazer (Press space to switch to stable only)";
        else if (whatClient.first) message = "stable (Press space to switch to lazer only)";
        else message = "lazer (Press space to switch to both)";

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("current client selected", 190, 150, 20, LIGHTGRAY);
        DrawText(message.c_str(), 190, 200, 20, LIGHTGRAY);
        EndDrawing();

        if(WindowShouldClose()) localIsRunning = false; 
    }
    isRunning.store(localIsRunning);
    return whatClient;
}

void run(const bool isSyncing, std::pair<bool,bool> whatClient, std::queue<std::string> &textQueue, std::mutex &mutex, std::condition_variable &queueNotify) {
    try {
        OsuPathUtility osuPathUtility(isSyncing, whatClient, textQueue, mutex, queueNotify);
        CompressionUtility zipper(isSyncing, textQueue, mutex, queueNotify);
        if (isSyncing) {
            zipper.Decompress();
            osuPathUtility.SyncOsuFiles();
        } else {
            osuPathUtility.CopyOsuFiles();
            zipper.Compress();
        }
    } catch(const std::exception& e) {
        throw std::invalid_argument(e.what());
    }
    queueNotify.notify_all();
}

int main(void) {
    InitWindow(800, 450, "Osu Sync");
    SetTargetFPS(60);

    std::atomic<bool> isRunning = true;

    std::mutex startMutex;
    std::condition_variable startNotify;

    waitNoLock(5);

    bool isSyncing = selectIfSync(isRunning);

    if (!isRunning.load()) return 0;

    std::pair<bool,bool> whatClient = selectClient(isRunning);

    if (!isRunning.load()) return 0;

    std::queue<std::string> textQueue;
    std::mutex queueMutex;
    std::condition_variable queueNotify;
    
    std::thread osuSyncThread(&run, isSyncing, whatClient, ref(textQueue), ref(queueMutex), ref(queueNotify));

    while (!WindowShouldClose() && isRunning.load())
    {
        std::vector<std::string> messages;

        std::unique_lock<std::mutex> lock(queueMutex);
        queueNotify.wait(lock, [&textQueue, &isRunning] {
            return !textQueue.empty() || !isRunning.load();
        });

        while (!textQueue.empty()) {
            messages.push_back(textQueue.front());
            textQueue.pop();
        }
        lock.unlock();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        while (!textQueue.empty()) {
            DrawText(messages.front().c_str(), 190, 200, 20, LIGHTGRAY);
            messages;
        }

        EndDrawing();

        std::scoped_lock<std::mutex> scoopedLock(queueMutex);
        if (osuSyncThread.joinable() && textQueue.empty()) isRunning.store(false);
    }

    osuSyncThread.join();
    
    CloseWindow();
    return 0;
}