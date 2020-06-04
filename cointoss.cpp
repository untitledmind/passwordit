#include <random>
#include <iostream>
#include <functional>
#include <chrono>

class Randomizer {
    private:
        unsigned seed;
        std::mt19937_64 gen;
    public:
        Randomizer(): seed(std::chrono::system_clock::now().time_since_epoch().count()),
            gen(seed) {}
        ~Randomizer() {}

        template<typename Type>
        Type choice(std::vector<Type>& vec)
        {
            std::uniform_int_distribution<int> dist(0, vec.size()-1);
            int index = dist(gen);
            return vec[index];
        }

        bool cointoss(void)
        {
            std::uniform_int_distribution<int> dist(0,1);
            bool result = dist(gen);
            return result;
        }

        void cointoss_trial(unsigned trials)
        {
            auto start = std::chrono::system_clock::now();
            std::uniform_int_distribution<int> dist(0,1);

            unsigned heads {0};
            unsigned tails {0};

            for (unsigned int i {0}; i<trials; ++i)
                heads += dist(gen);
            
            tails = trials - heads;

            auto stop = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

            std::cout << "Trials: " << trials << '\n';
            std::cout << "Elapsed: " << duration.count() << " s\n";
            std::cout << "Heads = " << static_cast<float>(heads) / trials << '\n';
            std::cout << "Tails = " << static_cast<float>(tails) / trials << '\n';
        }
};


int main()
{
    Randomizer random;
    random.cointoss_trial(1e9);

    return 0;
}