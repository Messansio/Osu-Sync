#include "OsuPathUtility.h"

OsuPathUtility::OsuPathUtility(const bool isSyncing) 
: osuType(0) {
    osuSPath.clear();
    osuLPath.clear();
    if (!isSyncing) askStableOrLazer();

    bool found = locateOsuFolder();

    if (!found) {
        std::cout << "Can't find your osu! folder.\n";
        exit(7270);
    }
}

void OsuPathUtility::CopyOsuFiles() {
    std::filesystem::path copyPath = std::filesystem::current_path();

    createDirectory(copyPath / "OsuFiles");
    copyPath.append("OsuFiles");

    if (osuType == 2 || osuType == 0) {
        createDirectory(copyPath / "Stable");
        const std::filesystem::path copyingPath = copyPath / "Stable";

        createDirectory(copyingPath / "Replays");
        createDirectory(copyingPath / "Songs");
        createDirectory(copyingPath / "Skins");

        std::cout << "Copying beatmaps..\n";
        copyFiles(osuSPath / "Songs", copyingPath / "Songs");

        std::cout << "Copying skins..\n";
        copyFiles(osuSPath / "Skins", copyingPath / "Skins");

        std::cout << "Copying replays..\n";
        copyFiles(osuSPath / "Replays", copyingPath / "Replays");

        std::cout << "Copying osu db..\n";
        copyFile(osuSPath / "osu!.db", copyingPath / "osu!.db");

        std::cout << "Copying scores db..\n";
        copyFile(osuSPath / "scores.db", copyingPath / "scores.db");

        std::cout << "Copying collection db..\n";
        copyFile(osuSPath / "collection.db", copyingPath / "collection.db");
    }
    if (osuType == 2 || osuType == 1) {
        createDirectory(copyPath / "Lazer");
        const std::filesystem::path copyingPath = copyPath / "Lazer";

        createDirectory(copyingPath / "files");

        std::cout << "Copying files..\n";
        copyFiles(osuLPath / "files", copyPath / "files");

        std::cout << "Copying realm..\n";
        copyFile(osuLPath / "client.realm", copyPath / "client.realm");
    }
    std::cout << "Copying complete\n";
}

void OsuPathUtility::SyncOsuFiles() {
    const std::filesystem::path copyPath = std::filesystem::current_path();
    const bool stableExists = std::filesystem::exists(copyPath / "OsuFiles" / "Stable");
    const bool lazerExists = std::filesystem::exists(copyPath / "OsuFiles" / "Lazer");
    const bool bothExists = stableExists && lazerExists;

    if (bothExists) osuType = 2;
    else if (lazerExists) osuType = 1;
    else if (stableExists) osuType = 0;
    else {
        std::cout << "Can't find your OsuFiles folder.\n Report this issue to GitHub.\n";
        exit(7272);
    }
    
    const std::filesystem::path syncPath = std::filesystem::current_path() / "OsuFiles";

    if (osuType == 2 || osuType == 0) {
    const std::filesystem::path syncingPath = syncPath / "Stable";

    std::cout << "Copying beatmaps..\n";
        copyFiles(syncingPath / "Songs", osuSPath / "Songs");

        std::cout << "Copying skins..\n";
        copyFiles(syncingPath / "Skins", osuSPath / "Skins");

        std::cout << "Copying replays..\n";
        copyFiles(syncingPath / "Replays", osuSPath / "Replays");

        std::cout << "Copying osu db..\n";
        copyFile(syncingPath / "osu!.db", osuSPath / "osu!.db");

        std::cout << "Copying scores db..\n";
        copyFile(syncingPath / "scores.db", osuSPath / "scores.db");

        std::cout << "Copying collection db..\n";
        copyFile(syncingPath / "collection.db", osuSPath / "collection.db");
    }
    if (osuType == 2 || osuType == 1) {
        const std::filesystem::path syncingPath = syncPath / "Lazer";

        std::cout << "Copying files..\n";
        copyFiles(syncingPath / "files", osuLPath / "files");

        std::cout << "Copying realm..\n";
        copyFile(syncingPath / "client.realm", osuLPath / "client.realm");
    }
    std::cout << "Copying complete\n";

    std::cout << "Deleting the cache files.\n";

    try {
        std::filesystem::remove_all(syncPath);
    } catch (const std::exception& e) {
        std::cerr << "Error deleting cache files: " << e.what() << '\n';
    }

    std::cout << "Cache Deleted.\n";
}

void OsuPathUtility::askStableOrLazer() {
    std::string temp;
    std::cout << "Chose which osu! you want to sync?\n0: Stable (default)\n1: Lazer\n2: Both\n";
    std::cout << "Your input: ";
    std::cin >> temp;
    std::cout << "\n";

    if (temp[0] == '2') {
        osuType = 2; // Both
    } else if (temp[0] == '1') {
        osuType = 1; // Lazer
    } else if (temp[0] == '0') {
        osuType = 0; // Stable
    } else {
        std::cout << "Invalid input, defaulting to Stable.\n";
        osuType = 0; // Default to Stable
    }
}

bool OsuPathUtility::locateOsuFolder() {
    bool noError = true;

    if (osuType == 2 || osuType == 0) {
        noError = locateOsuSFolder();
        if (!noError) return noError;
        else std::cout << "Found osu!stable directory at " << osuSPath.generic_string() << "\n";
    }

    if (osuType == 2 || osuType == 1) {
        noError = locateOsuLFolder();
        if (!noError) return noError;
        else std::cout << "Found osu!lazer directory at " << osuLPath.generic_string() << "\n";
    }

    return noError;
}

bool OsuPathUtility::locateOsuSFolder() {
    osuSPath = getenv("LOCALAPPDATA");
    osuSPath.append("osu!");
    return std::filesystem::exists(osuSPath);
}

bool OsuPathUtility::locateOsuLFolder() {
    osuLPath = getenv("APPDATA");
    osuLPath.append("osu");
    return std::filesystem::exists(osuLPath);
}

void OsuPathUtility::createDirectory(const std::filesystem::path& directory) {
    if (std::filesystem::exists(directory)) return;

    try {
        std::filesystem::create_directories(directory);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

void OsuPathUtility::copyFiles(const std::filesystem::path& source, const std::filesystem::path& destination) const {
    try {
        std::filesystem::copy(source, destination, std::filesystem::copy_options::recursive | std::filesystem::copy_options::update_existing);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

void OsuPathUtility::copyFile(const std::filesystem::path& source, const std::filesystem::path& destination) const {
    try {
    std::filesystem::copy_file(source, destination, std::filesystem::copy_options::update_existing);
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}