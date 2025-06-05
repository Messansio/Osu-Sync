#include <OsuPathUtility.h>
#include <CompressionUtility.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>

bool inputValidation(const std::string& input) {
    return input[0] == 'y' || input[0] == 'Y' || input[0] == 'n' || input[0] == 'N';
}

bool askIfSync() {
    std::string temp;
    std::cout << "Do you want to sync your osu! files? (y/n): ";
    std::cin >> temp;
    std::cout << "\n";

    bool isInputValid = inputValidation(temp);

    while (!isInputValid) {
        std::cout << "Invalid input. Please enter 'y' or 'n': ";
        std::cin >> temp;
        isInputValid = inputValidation(temp);
        std::cout << "\n";
    }

    return temp[0] == 'y' || temp[0] == 'Y';
}

std::pair<bool,bool> askStableOrLazer() {
    std::cout<<"inizio funzione \n";
    bool isLazer = true;
    bool isStable = false;
    std::cout<<"booleani \n";
    #if defined(_WIN32) || defined(_WIN64)
    std::string temp;
    std::cout << "Chose which osu! you want to sync?\n0: Stable (default)\n1: Lazer\n2: Both\n";
    std::cout << "Your input: ";
    std::cin >> temp;
    std::cout << "\n";
    if (temp[0] == '2' || temp[0] == '1') {
        isLazer = true;
    } else {
        isLazer = false;
    }
    
    if (temp[0] == '2' || temp[0] == '0') {
        isStable = true;
    } else {
        isStable = false;
    }

    if (!isStable && !isLazer) {
        std::cout << "Invalid input, defaulting to Stable.\n";
        isStable = true;
        isLazer = false;
    }
    #endif
    std::cout<<"questo è linux!!\n";
    return std::make_pair(isStable, isLazer);
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
        std::cout << e.what() << "\n";
    }
    queueNotify.notify_all();
}


void GitHubLink() {
    std::cout << "GitHub: https://github.com/Messansio/Osu-Sync.git\n";
}

int main(void) {
    std::cout << "Welcome to OsuSync!\n";

    std::cout << "This tool will help you sync your osu! files, " <<
                 "and it works with both clients, stable and lazer.\n";

    std::cout << "This tool is made by the naidea team.\n";
    std::cout << "we're not affiliated with osu! or their developers developers.\n";

    std::cout << "If you have any issues, please report them on the GitHub repository.\n";
    GitHubLink();
    std::cout << "\n" ;
    std::cout << "\n" ;

    const bool isSyncing = askIfSync();
    std::pair<bool,bool> whatClient = askStableOrLazer();

    std::queue<std::string> textQueue;
    std::mutex queueMutex;
    std::condition_variable queueNotify;
    
    std::thread osuSyncThread(&run, isSyncing, whatClient, ref(textQueue), ref(queueMutex), ref(queueNotify));

    std::atomic<bool> running = true;

    while(running) {
        std::unique_lock<std::mutex> lock(queueMutex);

        while(!textQueue.empty()) {
        std::cout << textQueue.front().c_str() << "\n";
        textQueue.pop();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        queueNotify.wait(lock, [&textQueue, &running] {
        return !textQueue.empty() || !running.load();
        });

        if (osuSyncThread.joinable() && textQueue.empty()) running = false;
    }

    osuSyncThread.join();

    std::cout << "If you liked it, pls star the project on GitHub.\n";
    GitHubLink();
    return 0; //exit no error
}