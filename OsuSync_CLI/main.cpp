#include <OsuPathUtility.h>
#include <CompressionUtility.h>

#ifdef DEBUG
    #include <iostream>
#endif

bool inputValidation(const char& input) {
    return input == 'y' || input == 'Y' || input == 'n' || input == 'N';
}

bool askIfSync() {
    std::string temp;
    std::cout << "Do you want to sync your osu! files? (y/n): ";
    std::cin >> temp;
    std::cout << "\n";

    bool isInputValid = inputValidation(temp.c_str()[0]);

    while (!isInputValid) {
        std::cout << "Invalid input. Please enter 'y' or 'n': ";
        std::cin >> temp;
        isInputValid = inputValidation(temp.c_str()[0]);
        std::cout << "\n";
    }

    const char answer = temp.c_str()[0];

    return answer == 'y' || answer == 'Y';
}

void GitHubLink() {
    std::cout << "GitHub: \n";
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
        osuPathUtility.CopyOsuFiles();
        zipper.Compress();
    } else {
        zipper.Decompress();
        osuPathUtility.SyncOsuFiles();
    }

    std::cout << "If you liked it, pls star the project on GitHub.\n";
    GitHubLink();

#ifdef DEBUG
    std::string debug;
    std::cin >> debug;
#endif
    return 0; //exit no error
}