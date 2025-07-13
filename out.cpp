#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

#define SEP std::cout<<"\n__________________________________________________________________________________\n"
#define SEP2 std::cout<<"\n-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-\n"
namespace fs = std::filesystem;


std::string text_from_file(const std::string &filename) {
    std::string result;
    std::fstream outfile(filename);
    if (outfile.is_open()) {
        std::string line;
        while (std::getline(outfile, line)) {
            result += line + "\n";
        }
    }
    return result;
}


std::vector<std::pair<std::string, std::string> > text_from_dir_current(const fs::path &dir) {
    std::vector<std::pair<std::string, std::string> > text;
    for (const auto &i: fs::directory_iterator(dir)) {
        if (i.is_regular_file()) {
            text.emplace_back(i.path().filename().string(), text_from_file(i.path().string()));
        }
    }
    return text;
}

std::vector<std::pair<std::string, std::string> > text_from_dir(const fs::path &dir) {
    std::vector<std::pair<std::string, std::string> > text;
    for (const auto &i: fs::directory_iterator(dir)) {
        if (i.is_regular_file()) {
            text.emplace_back(i.path().filename().string(), text_from_file(i.path().string()));
        } else if (i.is_directory()) {
            const auto res = text_from_dir(i.path());
            for (const auto &j: res) {
                text.emplace_back(j);
            }
        }
    }
    return text;
}


int main(const int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: out <*filename> | * | **\n";
    } else {
        std::vector<std::string> args;
        for (int i = 1; i < argc; i++) {
            args.emplace_back(argv[i]);
        }
        for (const auto &i: args) {
            if (i == "*") {
                const auto res = text_from_dir_current(fs::current_path());
                for (const auto &[i, text]: res) {
                    SEP;
                    std::cout << "filename: " << i << std::endl;
                    SEP2;
                    std::cout << text << std::endl;
                    SEP;
                }
            } else if (i == "**") {
                const auto res = text_from_dir(fs::current_path());
                for (const auto &[i, text]: res) {
                    SEP;
                    std::cout << "filename: " << i << std::endl;
                    SEP2;
                    std::cout << text << std::endl;
                    SEP;
                }
            } else {
                SEP;
                std::cout << "filename: " << i << std::endl;
                SEP2;
                std::cout << text_from_file(i) << std::endl;
                SEP;
            }
        }
    }
}
