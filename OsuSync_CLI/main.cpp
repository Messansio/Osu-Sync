#include <OsuPathUtility.h>
#include <CompressionUtility.h>
#include <iostream>

bool inputValidation(const std::string& input) {
    return input[0] == 'y' || input[0] == 'Y' || input[0] == 'n' || input[0] == 'N';
}

bool askIfSync() {
    std::string temp;
    std::cout << "Do you want to sync your osu! files? (y/n): ";
    std::cin >> temp;
    std::cout << "\n";

    bool isInputValid = inputValidation(temp);

    while (!isInputValid) {
        std::cout << "Invalid input. Please enter 'y' or 'n': ";
        std::cin >> temp;
        isInputValid = inputValidation(temp);
        std::cout << "\n";
    }

    return temp[0] == 'y' || temp[0] == 'Y';
}

void GitHubLink() {
    std::cout << "GitHub: https://github.com/Messansio/Osu-Sync.git\n";
}

int main(void) {
    std::cout << "Welcome to OsuSync!\n";

    std::cout << "This tool will help you sync your osu! files, " <<
                 "and it works with both clients, stable and lazer.\n";

    std::cout << "This tool is made by the naidea team.\n";
    std::cout << "we're not affiliated with osu! or their developers developers.\n";

    std::cout << "If you have any issues, please report them on the GitHub repository.\n";
    GitHubLink();

    const bool isSyncing = askIfSync();

    OsuPathUtility osuPathUtility(isSyncing);
    CompressionUtility zipper(isSyncing);
    if (isSyncing) {
        zipper.Decompress();
        osuPathUtility.SyncOsuFiles();
    } else {
        osuPathUtility.CopyOsuFiles();
        zipper.Compress();
    }

    std::cout << "If you liked it, pls star the project on GitHub.\n";
    GitHubLink();
    return 0; //exit no error
}