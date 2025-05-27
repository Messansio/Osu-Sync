#pragma once
#ifndef COMPRESSION_UTILITY_H
#define COMPRESSION_UTILITY_H

#include <filesystem>

class CompressionUtility {
public:
    CompressionUtility(const bool& isSyncing);
    
    void Compress();
    void Decompress();
private:
    const std::filesystem::path currentPath;
    const std::filesystem::path sevenZipFilePath;
};
#endif // COMPRESSION_UTILITY_H