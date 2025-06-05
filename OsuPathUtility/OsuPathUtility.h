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
    OsuPathUtility(const bool isSyncing, const std::pair<bool,bool> whatClient, std::queue<std::string> &stringQueue, std::mutex &mutex, std::condition_variable &queueCV);

    void CopyOsuFiles();
    void SyncOsuFiles();
private:
    std::unique_ptr<std::queue<std::string>> textQueue;
    std::unique_ptr<std::mutex> queueMutex;
    std::unique_ptr<std::condition_variable> queueNotify;

    const std::pair<bool,bool> osuType;

    std::filesystem::path osuSPath;
    std::filesystem::path osuLPath;

    bool locateOsuFolder();
    bool locateOsuSFolder();
    bool locateOsuLFolder();

    void createDirectory(const std::filesystem::path& directory);
    void copyFiles(const std::filesystem::path& source, const std::filesystem::path& destination) const;
    void copyFile(const std::filesystem::path& source, const std::filesystem::path& destination) const;

    void sendText(const std::string text);
};
#endif // OSU_PATH_TOOLS_H