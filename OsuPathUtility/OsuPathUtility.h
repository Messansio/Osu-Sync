#pragma once
#ifndef OSU_PATH_UTILITY_H
#define OSU_PATH_UTILITY_H

#include <filesystem>
#include <string>
#include <mutex>
#include <queue>
#include <condition_variable>

class OsuPathUtility {
public:
    OsuPathUtility(bool isSyncing, std::pair<bool,bool> whatClient, std::queue<std::string> &stringQueue, std::mutex &mutex, std::condition_variable &queueCV);

    void LocateOsuPath();
    void CopyOsuFiles();
    void SyncOsuFiles();
private:
    std::shared_ptr<std::queue<std::string>> textQueue;
    std::shared_ptr<std::mutex> queueMutex;
    std::shared_ptr<std::condition_variable> queueNotify;

    const std::pair<bool,bool> osuType;

    std::filesystem::path osuSPath;
    std::filesystem::path osuLPath;

    void locateOsuSFolder();
    void locateOsuLFolder();

    void createDirectory(const std::filesystem::path& directory);
    void copyFiles(const std::filesystem::path& source, const std::filesystem::path& destination) const;
    void copyFile(const std::filesystem::path& source, const std::filesystem::path& destination) const;

    void sendText(const std::string text);
};
#endif // OSU_PATH_TOOLS_H