#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

inline void printHelp() {
    using std::cout;
    using std::endl;

    cout<<"Usage:\n";
    cout<<"fcat <file> | :<directory> | ::<path/to/!file>\nWhere:";
    cout<<"file - file that will be created\n";
    cout<<"directory - directory that will be created\n";
    cout<<"path - path that will be created\n";
    cout<<"Example: \nfcat main.cpp text.txt :directory1 ::directory1/directory2 ::directory1/!file.txt\n";
}


inline std::vector<std::string> split(const std::string &str, const char delimiter) {
    std::vector<std::string> result;
    std::string temp;
    for (const char ch: str) {
        if (ch == delimiter) {
            result.push_back(temp);
            temp.clear();
        } else {
            temp += ch;
        }
    }
    result.push_back(temp); // добавить последний фрагмент
    return result;
}

inline void createDirectory(std::string &name) {
    name = name.substr(1);
    const bool res = fs::create_directory(name);
    if (res) {
        std::cout << "* <D> Directory " << name << " was created\n";
    } else {
        std::cout << "! <D> Directory " << name << " won`t be created\n";
    }
}

inline void createFile(const std::string &file) {
    const std::ofstream ifs(file);
    if (ifs) {
        std::cout << "* <F> File " << file << " was created\n";
    } else {
        std::cout << "! <F> File " << file << " won`t be created\n";
    }
}


inline void createPath(const std::string &path) {
    std::vector<std::string> entities = split(path, '/');
    std::string file;
    std::vector<std::string> dirs;
    for (std::string &i: entities) {
        std::cout<<i<<std::endl;
        if (i.starts_with(":")) {
            while (i.starts_with(":")) {
                i = i.substr(1);
            }
            dirs.push_back(i);
        } else if (i.starts_with("!")) {
            file = i.substr(1);
        } else {
            dirs.push_back(i);
        }
    }
    fs::path path_dirs;
    for (const auto &dir : dirs) {
        path_dirs /= dir;
    }

    const bool res = fs::create_directories(path_dirs);
    if (res) {
        std::cout << "* <P> Path " << path_dirs << " was created\n";
    } else {
        std::cout << "! <P> Path " << path_dirs << " was not created\n";
    }
    if (!file.empty()) {
        path_dirs /= file;
        const std::ofstream ofs(path_dirs);
        if (ofs) {
            std::cout << "* <F> File " << file << " was created\n";
        } else {
            std::cout << "! <F> File " << file << " won`t be created\n";
        }
    }
}


inline void parse(const int argc, char **argv) {
    std::vector<std::string> args{};
    for (int i = 1; i < argc; i++) {
        args.emplace_back(argv[i]);
    }


    // parse
    for (auto &i: args) {

        if (i.starts_with("::")) {
            createPath(i);
        }
        else if (i.starts_with(":")) {
            createDirectory(i);
        }
        else {
            createFile(i);
        }
    }
}


int main(const int argc, char **argv) {

    if (argc <2) {
        printHelp();
    }
    parse(argc, argv);
    return 0;
}
