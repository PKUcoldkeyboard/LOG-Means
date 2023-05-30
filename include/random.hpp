#ifndef __RANDOM_HPP__
#define __RANDOM_HPP__
#include <random>

class Random {
public:
    Random() : eng(std::random_device{}()), uniform_dist(0, 1) {}

    Random(unsigned int seed) : eng(seed), uniform_dist(0, 1) {}
    
    float randn() {
        // 生成0到1之间的浮点数
        return uniform_dist(eng);
    }

    int randint(int a, int b) {
        // 生成[a, b]的随机整数
        std::uniform_int_distribution<> dist(a, b);
        return dist(eng);
    }

    // 用于生成具有离散分布的随机整数
    template <typename Iter>
    int rand_discrete(Iter begin, Iter end) {
        std::discrete_distribution<> dist(begin, end);
        return dist(eng);
    }

    // 提供引擎供外部使用
    std::mt19937 &get_engine() {
        return eng;
    }
    
private:
    std::mt19937 eng;
    std::uniform_real_distribution<float> uniform_dist;
};

#endif // __RANDOM_HPP__