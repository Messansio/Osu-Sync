#pragma once
#ifndef COMPRESSION_UTILITY_H
#define COMPRESSION_UTILITY_H

#include <filesystem>
#include <string>
#include <mutex>
#include <queue>
#include <condition_variable>

class CompressionUtility {
public:
    CompressionUtility(const bool& isSyncing, std::queue<std::string> &stringQueue, std::mutex &mutex, std::condition_variable &queueCV);
    
    void Compress();
    void Decompress();
private:
    std::unique_ptr<std::queue<std::string>> textQueue;
    std::unique_ptr<std::mutex> queueMutex;
    std::unique_ptr<std::condition_variable> queueNotify;
    void sendText(const std::string text);
    const std::filesystem::path currentPath;
    const std::filesystem::path sevenZipFilePath;
};
#endif // COMPRESSION_UTILITY_H