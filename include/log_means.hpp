#ifndef __LOG__MEANS__HPP__
#define __LOG__MEANS__HPP__
#include "kmeans.hpp"
#include "utils.hpp"
#include <queue>
#include <unordered_map>

class LogMeans {
public:
    /**
     * 预估数据集簇的数量
     * @param data 数据集
     * @param klow 最小簇数量
     * @param khigh 最大簇数量
     * @param epsilon 邻居数量
     * @return 预估的簇数量
     *
    */
    template<typename Scalar>
    int run(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> &data, int k_low, int k_high, int epsilon = 0);

private:
    bool directly_adjacent(int k_low, int k_high) {
        return k_low + 1 == k_high || k_low == k_high + 1 || k_low == k_high;
    }
    
    template<typename Scalar>
    int find_max (std::unordered_map<int, Scalar> &map) {
        Scalar max = 0;
        int key = 0;
        for (auto & [k, v] : map) {
            if (v > max) {
                max = v;
                key = k;
            }
        }
        return key;
    }

    template<typename Scalar>
    int find_adjacent(std::unordered_map<int, Scalar> &map, int k_high) {
        // 找到map中key比k小的最接近khigh的key
        int key = 0;
        Scalar min = 0;
        for (auto & [k, v] : map) {
            if (k < k_high) {
                if (min == 0) {
                    min = v;
                    key = k;
                } else {
                    if (v < min) {
                        min = v;
                        key = k;
                    }
                }
            }
        }
        return key;   
    }
};

template<typename Scalar>
int LogMeans::run(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> &data, int k_low, int k_high, int epsilon) {
    int k_min = -1;
    Scalar min_sse = std::numeric_limits<Scalar>::max();
    for (int k = k_low; k <= k_high; k++) {
        KMeans kmeans(k);
        auto centroids = kmeans.fit<Scalar>(data);
        auto sse = utils::compute_sse<Scalar>(data, centroids);
        std::cout << "k: " << k << ", sse: " << sse << std::endl;
        if (sse < min_sse) {
            min_sse = sse;
            k_min = k;
        }
    }
    return k_min;
}

#endif // __LOG__MEANS__HPP__