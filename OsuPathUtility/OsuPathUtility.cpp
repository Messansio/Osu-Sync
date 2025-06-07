#include "OsuPathUtility.h"

#include <stdexcept>
#include <iostream>

OsuPathUtility::OsuPathUtility(bool isSyncing, std::pair<bool,bool> whatClient, std::queue<std::string> &stringQueue, std::mutex &mutex, std::condition_variable &queueCV) 
: osuType(whatClient), textQueue(&stringQueue), queueMutex(&mutex), queueNotify(&queueCV) {
    osuSPath.clear();
    osuLPath.clear();
}

void OsuPathUtility::LocateOsuPath() {
    #if defined(_WIN32) || defined(_WIN64)
    if (osuType.first) {
        locateOsuSFolder();
        std::string sPathText = "Found osu!stable directory at ";
        sendText(sPathText.append(osuSPath.generic_string()));
    }
    #endif

    if (osuType.second) {
        locateOsuLFolder();
        std::string lPathText = "Found osu!lazer directory at ";
        sendText(lPathText.append(osuLPath.generic_string()));
    }
}

void OsuPathUtility::CopyOsuFiles() {
    std::filesystem::path copyPath = std::filesystem::current_path();

    createDirectory(copyPath / "OsuFiles");
    copyPath.append("OsuFiles");

    #if defined(_WIN32) || defined(_WIN64)
    if (osuType.first) {
        createDirectory(copyPath / "Stable");
        const std::filesystem::path copyingPath = copyPath / "Stable";

        createDirectory(copyingPath / "Replays");
        createDirectory(copyingPath / "Songs");
        createDirectory(copyingPath / "Skins");

        sendText("Copying beatmaps..");
        copyFiles(osuSPath / "Songs", copyingPath / "Songs");

        sendText("Copying skins..");
        copyFiles(osuSPath / "Skins", copyingPath / "Skins");

        sendText("Copying replays..");
        copyFiles(osuSPath / "Replays", copyingPath / "Replays");

        sendText("Copying osu db..");
        copyFile(osuSPath / "osu!.db", copyingPath / "osu!.db");

        sendText("Copying scores db..");
        copyFile(osuSPath / "scores.db", copyingPath / "scores.db");

        sendText("Copying collection db..");
        copyFile(osuSPath / "collection.db", copyingPath / "collection.db");
    }
    #endif
    if (osuType.second) {
        createDirectory(copyPath / "Lazer");
        const std::filesystem::path copyingPath = copyPath / "Lazer";

        createDirectory(copyingPath / "files");

        sendText("Copying files..");
        copyFiles(osuLPath / "files", copyingPath / "files");

        sendText("Copying realm..");
        copyFile(osuLPath / "client.realm", copyingPath / "client.realm");
    }
    sendText("Copying complete!");
}

void OsuPathUtility::SyncOsuFiles() {
    const std::filesystem::path copyPath = std::filesystem::current_path();

    #if defined(_WIN32) || defined(_WIN64)
    const bool clientsExists = std::filesystem::exists(copyPath / "OsuFiles" / "Stable") && osuType.first
                            || std::filesystem::exists(copyPath / "OsuFiles" / "Lazer") && osuType.second;
    #else
    const bool clientsExists = std::filesystem::exists(copyPath / "OsuFiles" / "Lazer") && osuType.second;
    #endif

    if (!clientsExists) {
        throw std::invalid_argument("Can't find your OsuFiles folder.");
    }
    
    const std::filesystem::path syncPath = std::filesystem::current_path() / "OsuFiles";
    
    #if defined(_WIN32) || defined(_WIN64)
    if (osuType.first) {
        const std::filesystem::path syncingPath = syncPath / "Stable";

        sendText("Syncing beatmaps..");
        copyFiles(syncingPath / "Songs", osuSPath / "Songs");

        sendText("Syncing skins..");
        copyFiles(syncingPath / "Skins", osuSPath / "Skins");

        sendText("Syncing replays..");
        copyFiles(syncingPath / "Replays", osuSPath / "Replays");

        sendText("Syncing osu db..");
        copyFile(syncingPath / "osu!.db", osuSPath / "osu!.db");

        sendText("Syncing scores db..");
        copyFile(syncingPath / "scores.db", osuSPath / "scores.db");

        sendText("Syncing collection db..");
        copyFile(syncingPath / "collection.db", osuSPath / "collection.db");
    }
    #endif
    if (osuType.second) {
        const std::filesystem::path syncingPath = syncPath / "Lazer";

        sendText("Syncing files..");
        copyFiles(syncingPath / "files", osuLPath / "files");

        sendText("Syncing realm..");
        copyFile(syncingPath / "client.realm", osuLPath / "client.realm");
    }
    sendText("Copying complete!");

    sendText("Deleting the cache files.");

    try {
        std::filesystem::remove_all(syncPath);
    } catch (const std::exception& e) {
        throw std::invalid_argument(e.what());
    }

    sendText("Cache Deleted!");
}

void OsuPathUtility::locateOsuSFolder() {
    #if defined(_WIN32) || defined(_WIN64)
    osuSPath = getenv("LOCALAPPDATA");
    osuSPath.append("osu!");

    if (!std::filesystem::exists(osuSPath)) {
        throw std::invalid_argument("Can't find your osu!Stable folder.");
    }
    #endif
}

void OsuPathUtility::locateOsuLFolder() {
    #if defined(_WIN32) || defined(_WIN64)
    osuLPath = getenv("APPDATA");
    osuLPath.append("osu");
    #else
        osuLPath = getenv("APPDATA");
        osuLPath.append("sh.ppy.osu");
        sendText(osuLPath.generic_string());
    #endif
    if (!std::filesystem::exists(osuLPath)) {
        throw std::invalid_argument("Can't find your osu!Lazer folder.");
    }
}

void OsuPathUtility::createDirectory(const std::filesystem::path& directory) {
    if (std::filesystem::exists(directory)) return;

    try {
        std::filesystem::create_directories(directory);
    }
    catch(const std::exception& e) {
        throw std::invalid_argument(e.what());
    }
}

void OsuPathUtility::copyFiles(const std::filesystem::path& source, const std::filesystem::path& destination) const {
    try {
        std::filesystem::copy(source, destination, std::filesystem::copy_options::recursive | std::filesystem::copy_options::update_existing);
    }
    catch(const std::exception& e) {
        throw std::invalid_argument(e.what());
    }
}

void OsuPathUtility::copyFile(const std::filesystem::path& source, const std::filesystem::path& destination) const {
    try {
    std::filesystem::copy_file(source, destination, std::filesystem::copy_options::update_existing);
    }
    catch(const std::exception& e) {
        throw std::invalid_argument(e.what());
    }
}

void OsuPathUtility::sendText(const std::string text) {
    std::lock_guard<std::mutex> lock(*queueMutex);
    textQueue->push(text);
    queueNotify->notify_one();
}