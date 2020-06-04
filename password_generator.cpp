#include <unordered_set>
#include <filesystem>
#include <vector>
#include <string>
#include <numeric>
#include <fstream>
#include <random>
#include <iostream>
#include <chrono>
#include <ratio>

class Timer {
    private:
        std::chrono::system_clock::time_point start_;
        std::chrono::system_clock::time_point stop_;
        std::chrono::duration<double, std::ratio<1>> duration_;
    public:
        Timer() {}
        ~Timer() {}
        void start() {start_ = std::chrono::system_clock::now();}
        void stop() {stop_ = std::chrono::system_clock::now(); duration_ = std::chrono::duration_cast<std::chrono::seconds>(stop_ - start_);}
        double elapsed() {return duration_.count();}
};

class Randomizer {
    private:
        unsigned seed;
        std::mt19937_64 generator;
    public:
        Randomizer(): seed(std::chrono::system_clock::now().time_since_epoch().count()),
            generator(seed) {}
        ~Randomizer() {}

        template<typename Type>
        Type choice(const std::vector<Type>& v)
        {
            std::uniform_int_distribution<int> U(0, v.size()-1);
            int i = U(generator);
            return v[i];
        }

        bool cointoss(void)
        {
            std::uniform_int_distribution<int> U(0,1);
            bool result = U(generator);
            return result;
        }

        void cointoss_trial(unsigned trials)
        {
            Timer timer;
            timer.start();
            std::uniform_int_distribution<int> U(0,1);
            unsigned heads {0};
            unsigned tails {0};

            for (unsigned int i {0}; i<trials; ++i)
                heads += U(generator);
            
            tails = trials - heads;

            timer.stop();
            std::cout << "Trials: " << trials << '\n';
            std::cout << "Elapsed: " << timer.elapsed() << " s\n";
            std::cout << "Heads = " << static_cast<float>(heads) / trials << '\n';
            std::cout << "Tails = " << static_cast<float>(tails) / trials << '\n';
        }
};

std::vector<std::string> get_files_in_current_dir(std::string ext_)
{
    namespace fs = std::filesystem;

    std::vector<std::string> matches;

    fs::path current_dir = fs::current_path();
    for (auto& file : fs::directory_iterator(current_dir)) {
        fs::path path = file.path();
        if (path.extension().string() == ext_)
            matches.push_back(path.filename());
    }
    return matches;
}

std::string pick_password_nato(void)
{
    std::vector<std::string> letters = {
        "Alfa", "Bravo", "Charlie", "Delta", "Echo", "Foxtrot", "Golf", "Hotel",
        "India", "Juliet", "Kilo", "Lima", "Mike", "November", "Oscar", "Papa",
        "Quebec", "Romeo", "Sierra", "Tango", "Uniform", "Victor", "Whiskey",
        "Xray", "Yankee", "Zulu"
    };
    std::vector<int> numbers;
    for (int i=11; i < 100; ++i)
        numbers.push_back(i);
    Randomizer random;
    bool can_pick_num = random.cointoss();
    int num {0};
    int lett {0};
    std::string password;

    do {
        if (can_pick_num) {
            password += std::to_string(random.choice(numbers));
            ++num;
            can_pick_num = false;
        } else {
            password += random.choice(letters);
            ++lett;
            can_pick_num = random.cointoss();
        }
    } while (lett < 2);
    
    if (num == 0)
        password += std::to_string(random.choice(numbers));
    
    return password;
}

// template<typename Type>
// void write_receipt(std::vector<Type>& vec) {
//     std::ofstream file("passwords.txt", std::ios::out, std::ios::app, std::ios::trunc);
//     file << ;
//     file << '\n';
// }

int main(void)
{
    std::string ext;
    std::cout << "File extension: ";
    std::cin >> ext;

    if (ext[0] != '.')
        ext.insert(0,1, '.');
    std::vector<std::string> files = get_files_in_current_dir(ext);

    std::ofstream file("passwords.txt"/*, std::ios::out, std::ios::app, std::ios::trunc*/);
    for (std::string& f : files) {
        std::string pword = pick_password_nato();
        std::cout << f << ": " << pword << '\n';
        file << f << ": " << pword << '\n';
    }

    return 0;
}