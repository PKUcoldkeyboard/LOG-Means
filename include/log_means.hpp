#ifndef __LOG__MEANS__HPP__
#define __LOG__MEANS__HPP__
#include "kmeans.hpp"
#include "utils.hpp"
#include <queue>
#include <unordered_map>

class LogMeans {
public:
    template<typename Scalar>
    Scalar kmeans_sse(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data, int k) {
        KMeans kmeans(k);
        auto centroids = kmeans.fit<Scalar>(data);
        return utils::compute_sse<Scalar>(data, centroids);
    }
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
    int run(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data, int k_low, int k_high, Scalar epsilon = 0);

private:
    bool directly_adjacent(int k_low, int k_high) {
        return k_low + 1 == k_high;
    }
};

template<typename Scalar>
int LogMeans::run(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data, int k_low, int k_high, Scalar epsilon) {
    k_low--;
    std::unordered_map<int, Scalar> K, M;
    Scalar sseLow = kmeans_sse<Scalar>(data, k_low);
    Scalar sseHigh = kmeans_sse<Scalar>(data, k_high);
    K[k_low] = sseLow;
    K[k_high] = sseHigh;
    while (k_low < k_high && !directly_adjacent(k_low, k_high)) {
        int k_mid = (k_low + k_high) / 2;
        std::cout << "k_Low=" << k_low << ",k_mid=" << k_mid << ",k_high=" << k_high << std::endl;
        Scalar sseMid = kmeans_sse(data, k_mid);
        K[k_mid] = sseMid;
        Scalar radioLeft = sseLow / sseMid;
        Scalar radioRight = sseMid / sseHigh;
        std::cout << "radioLeft=" << radioLeft << ", radioRight=" << radioRight << std::endl;
        M[k_mid] = radioLeft;
        M[k_high] = radioRight;
        int temp_klow = k_low; // 1
        int temp_khigh = k_high; // 24
        Scalar maxRadio = 0;
        for (const auto &[k, v]: M) {
            if (v > maxRadio) {
                maxRadio = v;
                k_high = k;
            }
        }
        for (int i = k_high - 1; i >= 0; i--) {
            if (K.count(i)) {
                k_low = i;
                break;
            }
        }
        sseHigh = K[k_high];
        sseLow = K[k_low];
    }
    int result = 0;
    Scalar maxRadio = 0;
    std::cout << "k_low: " << k_low << ", k_high: " << k_high << std::endl;
    for (const auto &[k, v]: M) {
        if (k >= k_low && k <= k_high && v > maxRadio) {
            maxRadio = v;
            result = k;
        }
    }
    return result;
}

#endif // __LOG__MEANS__HPP__