#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>


namespace fs = std::filesystem;


int calculate_sum(const std::string &filename) {
    std::fstream file{filename};
    if (file.is_open()) {
        std::string text;
        std::string line;
        int sum = 0;
        while (getline(file, line)) {
            text += line + "\n";
            sum += 1;
        }
        return sum;
    }
    return 0;
}


int calculate_in_dir(const fs::path &dir) {
    int sum = 0;
    for (const auto &i: fs::directory_iterator(dir)) {
        if (i.is_regular_file()) {
            const int res = calculate_sum(i.path().string());
            std::cout << "\t<F> " << "lines: " << res << " :: file: " << i.path().string() << std::endl;
            sum += res;
        }
    }

    return sum;
}

int calculate_in_dir2(const fs::path &dir) {
    int sum = 0;
    for (const auto &i: fs::directory_iterator(dir)) {
        if (i.is_regular_file()) {
            const int res = calculate_sum(i.path().string());
            std::cout << "\t<F> " << "lines: " << res << " :: file: " << i.path().string() << std::endl;
            sum += res;
        } else if (i.is_directory()) {
            const int res = calculate_in_dir2(i.path());
            std::cout << "\t<D> " << "lines: " << res << " :: file: " << i.path().string() << std::endl;
            sum += res;
        }
    }

    return sum;
}


int main(const int argc, char **argv) {
    if (argc > 1) {
        std::vector<std::string> args;
        for (int i = 1; i < argc; i++) {
            args.emplace_back(argv[i]);
        }
        int sum = 0;
        for (const auto &arg: args) {
            if (arg == "*") {
                const int result = calculate_in_dir(fs::current_path());
                sum += result;
            } else if (arg == "**") {
                const int result = calculate_in_dir2(fs::current_path());
                sum += result;
            } else {
                const int result = calculate_sum(arg);
                std::cout << "\t<F> " << "lines: " << result << " :: file: " << arg << std::endl;
                sum += result;
            }
        }
        std::cout << "Total: " << sum << std::endl;
    } else {
        std::cout << "Usage: flc <*filename> | * | ** \n";
    }
    return 0;
}
