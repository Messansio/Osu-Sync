#include "CompressionUtility.h"
#include <iostream>

#include <bit7z/bitarchivewriter.hpp>
#include <bit7z/bitfileextractor.hpp>

#if defined(_WIN32) || defined(_WIN64)
#define SEVEN_Z_PATH "C:/Program Files/7-Zip/7z.dll"
#else
#define SEVEN_Z_PATH "/usr/lib/7z.so"
#endif

CompressionUtility::CompressionUtility(const bool& isSyncing, std::queue<std::string> &stringQueue, std::mutex &mutex, std::condition_variable &queueCV)
: currentPath(std::filesystem::current_path()), sevenZipFilePath(SEVEN_Z_PATH), textQueue(&stringQueue), queueMutex(&mutex), queueNotify(&queueCV) {
    if (!std::filesystem::exists(sevenZipFilePath)) {
        throw std::invalid_argument("Can't find your 7z.dll file. Make sure you have 7zip downloaded.");
    }
}

void CompressionUtility::Compress() {
    if (!std::filesystem::exists(currentPath / "OsuFiles")) {
        throw std::invalid_argument("Can't find your OsuFiles folder. Report this issue to GitHub.");
    }

    const std::filesystem::path activePath = currentPath / "OsuFiles";

    sendText("Inizializing Files Compression.\n");

    try {
        bit7z::Bit7zLibrary lib{sevenZipFilePath.generic_string()};

        bit7z::BitArchiveWriter archive{lib, bit7z::BitFormat::SevenZip};

        sendText("addDirectory.");

        archive.addDirectory(activePath.generic_string());

        sendText("Compressing.");
    
        archive.compressTo( "OsuFiles.syncfile" );

    } catch ( const bit7z::BitException& ex ) {
        throw std::invalid_argument(ex.what());
    }

    sendText("Compression Complete.");

    sendText("Deleting the cache files.");

    try {
        std::filesystem::remove_all(activePath);
    } catch (const std::exception& e) {
        throw std::invalid_argument(e.what());
    }

    sendText("Cache Deleted.");
}

void CompressionUtility::Decompress() {
    if (!std::filesystem::exists(currentPath / "OsuFiles.syncfile")) {
        throw std::invalid_argument("Can't find your syncfile.\n Make sure you have |OsuFiles.syncfile| in the same directory and named properly.");
    }

    const std::filesystem::path activePath = currentPath;
    const std::filesystem::path filePath = currentPath / "OsuFiles.syncfile";

    sendText("Decompressing.");

    try {
        bit7z::Bit7zLibrary lib{sevenZipFilePath.generic_string()};
        bit7z::BitFileExtractor extractor{lib, bit7z::BitFormat::SevenZip};

        extractor.extract(currentPath.generic_string(), filePath.generic_string());
    }
    catch(const bit7z::BitException& ex) {
        throw std::invalid_argument(ex.what());
    }

    sendText("Decompression Complete.");

    sendText("Deleting the cache files.");

    try {
        std::filesystem::remove(filePath);
    } catch (const std::exception& e) {
        throw std::invalid_argument(e.what());
    }

    sendText("Cache Deleted.");
}

void CompressionUtility::sendText(const std::string text) {
    std::lock_guard<std::mutex> lock(*queueMutex);
    textQueue->push(text);
    queueNotify->notify_one();
}