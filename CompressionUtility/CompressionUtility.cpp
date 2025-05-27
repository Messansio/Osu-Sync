#include "CompressionUtility.h"
#include <iostream>

#include <bit7z/bitarchivewriter.hpp>
#include <bit7z/bitfileextractor.hpp>

CompressionUtility::CompressionUtility(const bool& isSyncing)
: currentPath(std::filesystem::current_path()) {
    if (isSyncing) {
        if (!std::filesystem::exists(currentPath / "OsuFiles.syncfile")) {
            std::cout << "Can't find your syncfile.\n Make sure you have |OsuFiles.syncfile| in the same directory and named properly.\n";
            exit(7271);
        }
    } else {
        if (!std::filesystem::exists(currentPath / "OsuFiles")) {
            std::cout << "Can't find your OsuFiles folder.\n Report this issue to GitHub.\n";
            exit(7272);
        }
    }
}

void CompressionUtility::Compress() {
    const std::filesystem::path activePath = currentPath / "OsuFiles";

    std::cout << "Inizializing Files Compression.\n";

    try {
        bit7z::Bit7zLibrary lib{"7z.dll"};

        bit7z::BitArchiveWriter archive{lib, bit7z::BitFormat::SevenZip};

        std::cout << "addDirectory.\n";

        archive.addDirectory(activePath.generic_string());

        std::cout << "Compressing.\n";
    
        archive.compressTo( "OsuFiles.syncfile" );

    } catch ( const bit7z::BitException& ex ) {
        std::cerr << ex.what() << '\n';
    }

    std::cout << "Compression Complete.\n";

    std::cout << "Deleting the cache files.\n";

    try {
        std::filesystem::remove_all(activePath);
    } catch (const std::exception& e) {
        std::cerr << "Error deleting cache files: " << e.what() << '\n';
    }

    std::cout << "Cache Deleted.\n";
}

void CompressionUtility::Decompress() {
    const std::filesystem::path activePath = currentPath;
    const std::filesystem::path filePath = currentPath / "OsuFiles.syncfile";

    std::cout << "Decompressing.\n";

    try {
        bit7z::Bit7zLibrary lib{"7z.dll"};
        bit7z::BitFileExtractor extractor{lib, bit7z::BitFormat::SevenZip};

        extractor.extract(currentPath.generic_string(), filePath.generic_string());
    }
    catch(const bit7z::BitException& ex) {
        std::cerr << ex.what() << '\n';
    }

    std::cout << "Decompression Complete.\n";

    std::cout << "Deleting the cache files.\n";

    try {
        std::filesystem::remove(filePath);
    } catch (const std::exception& e) {
        std::cerr << "Error deleting cache files: " << e.what() << '\n';
    }

    std::cout << "Cache Deleted.\n";
}