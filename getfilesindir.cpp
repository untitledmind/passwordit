#include <vector>
#include <string>
#include <iostream>
#include <filesystem>

std::vector<std::string> get_files_in_current_dir(std::string ext_)
{
    namespace fs = std::filesystem;

    std::vector<std::string> matches;

    fs::path currentdir = fs::current_path();
    for (auto& file : fs::directory_iterator(currentdir)) {
        fs::path path = file.path();
        if (path.extension().string() == ext_)
            matches.push_back(path.filename());
    }
    return matches;
}

int main(void)
{
    std::string ext;
    std::cout << "File extension: ";
    std::cin >> ext;
    if (ext[0] != '.')
        ext.insert(0,1, '.');
    std::vector<std::string> results = get_files_in_current_dir(ext);

    for (auto& i : results)
        std::cout << i << '\n';

    return 0;
}