#ifndef __RANDOM_HPP__
#define __RANDOM_HPP__
#include <cstdlib>
#include <ctime>

class Random {
public:
    Random() {
        srand((unsigned int) time(NULL));
    }
    float randn() {
        // 生成0到1之间的浮点数
        return static_cast<float>(rand()) / RAND_MAX;
    }
    int randint(int a, int b) {
        // 生成[a, b]的随机整数
        return rand() % (b - a + 1) + a;
    }
};

#endif // __RANDOM_HPP__