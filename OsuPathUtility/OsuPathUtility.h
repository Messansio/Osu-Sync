#pragma once
#ifndef OSUPATHUTILITY_H
#define OSUPATHUTILITY_H

#include <filesystem>
#include <iostream>

class OsuPathUtility {
public:
    OsuPathUtility(const bool isSyncing);
    void CopyOsuFiles();
    void SyncOsuFiles();
private:
    unsigned int osuType : 2;
    std::filesystem::path osuSPath;
    std::filesystem::path osuLPath;
    void askStableOrLazer();
    bool locateOsuFolder();
    bool locateOsuSFolder();
    bool locateOsuLFolder();
    void createDirectory(const std::filesystem::path& directory);
    void copyFiles(const std::filesystem::path& source, const std::filesystem::path& destination) const;
    void copyFile(const std::filesystem::path& source, const std::filesystem::path& destination) const;
};
#endif // OSU_PATH_TOOLS_H