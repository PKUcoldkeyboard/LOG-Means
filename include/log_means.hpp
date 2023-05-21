#ifndef __LOG__MEANS__HPP__
#define __LOG__MEANS__HPP__
#include "kmeans.hpp"
#include "utils.hpp"

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
    int run(std::vector<Eigen::VectorXd> &data, int k_low, int k_high, int epsilon = 0);
private:

};

int LogMeans::run(std::vector<Eigen::VectorXd> &data, int k_low, int k_high, int epsilon) {
    k_low = k_low - 1;
    KMeans kmeans(k_low);
    auto clusters = kmeans.fit(data);
    auto sse_low = utils::compute_sse(clusters.first);
    kmeans.set_k(k_high);
    clusters = kmeans.fit(data);
    auto sse_high = utils::compute_sse(clusters.first);
}

#endif // __LOG__MEANS__HPP__