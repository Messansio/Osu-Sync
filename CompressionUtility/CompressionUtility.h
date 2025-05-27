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
};
#endif // COMPRESSION_UTILITY_H