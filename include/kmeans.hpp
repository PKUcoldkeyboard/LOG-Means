#ifndef __KMEANS_HPP__
#define __KMEANS_HPP__
#include <eigen3/Eigen/Dense>
#include <vector>
#include <omp.h>
#include "random.hpp"
#include "utils.hpp"
#include "spdlog_common.h"

class KMeans {
public:
    KMeans(int k, int max_iterations = 20, unsigned int seed = 42) : k(k), max_iterations(max_iterations), random(seed) {}
    void set_k(int k) { 
        this->k = k; 
    }

    /**
     * @brief KMeans++初始化质心
     * @param data: 数据集
     * @return: 初始化后的质心
    */
    template<typename Scalar>
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> init_centroids(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data);

    /**
     * @brief 计算点到最近质心的距离
     * @param centroids: 质心集合
     * @param point: 数据点
     * @return: 点到最近质心的距离平方
    */
    template<typename Scalar>
    double point_cost(const std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> &centroids, const Eigen::RowVector<Scalar, Eigen::Dynamic> &point);

    /**
     * @brief KMeans聚类
     * @param data: 数据集
     * @return: 聚类后的质心
    */
    template<typename Scalar>
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> fit(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data);

private:
    // cluster number
    int k;
    // max iterations
    int max_iterations;
    Random random;
};

template<typename Scalar>
std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> KMeans::init_centroids(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data) {
    int n = data.rows();
    int dim = data.cols();

    // 随机选取第一个质心
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> centroids;
    int c0 = random.randint(0, n - 1);
    centroids.emplace_back(data.row(c0));

    // KMeansⅡ选取方法
    // r为超参数，一般取lgn, spark默认取5， l=2k
    // const int r = (int)std::log10(n);
    const int r = 5;
    const int l = 2 * k;
    for (int round = 0; round < r; round++) {
        std::vector<double> minDists(n, 0);
        // Scalar sum = 0; 无论float还是double都用double存和，因为KITSUNE可能会溢出
        double sum = 0;
        #pragma omp parallel for reduction(+:sum)
        for (int i = 0; i < n; i++) {
            minDists[i] = point_cost<Scalar>(centroids, data.row(i));
            sum += minDists[i];
        }
        // 加权概率选取质心
        for (int i = 0; i < n; i++) {
            double prob = l * minDists[i] / sum;
            if (random.randn() < prob) {
                centroids.emplace_back(data.row(i));
            }
        }
    }

    // 候选质心数小于k，则返回所有质心
    if (centroids.size() <= k) {
        SPDLOG_INFO("centroids size: {} <= {}", centroids.size(), k);
        return centroids;
    }

    // 可能大于k个质心，继续后续处理，先计算出权重：表示距离x点最近的点的个数
    std::vector<int> weights(centroids.size(), 0);
    #pragma omp parallel for
    for (int i = 0; i < centroids.size(); i++) {
        int minIndex = 0;
        double minDist = (centroids[i] - centroids[0]).squaredNorm();
        for (int j = 1; j < centroids.size(); j++) {
            double dist = (centroids[i] - centroids[j]).squaredNorm();
            if (dist < minDist) {
                minDist = dist;
                minIndex = j;
            }
        }
        weights[minIndex]++;
    }

    // recluster过程：根据weights权重选取k个质心
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> newCentroids;
    // 先选取一个质心
    c0 = random.rand_descrete(weights.begin(), weights.end());
    newCentroids.emplace_back(centroids[c0]);

    // 根据距离和权重选择其余的质心
    for (int i = 1; i < k; i++) {
        std::vector<double> minDists(centroids.size(), 0);
        // 计算每个候选质心到已选质心集合的最小距离
        #pragma omp parallel for
        for (int j = 0; j < centroids.size(); j++) {
            minDists[j] = point_cost<Scalar>(newCentroids, centroids[j]);
        }
        // 计算每个候选质心被选为新质心的权重（距离 * 候选质心权重）
        std::vector<double> probs(centroids.size(), 0);
        double sum = 0;
        #pragma omp parallel for reduction(+:sum)
        for (int j = 0; j < centroids.size(); j++) {
            probs[j] = minDists[j] * weights[j];
            sum += probs[j];
        }
        // 根据权重选择新的质心
        int c = random.rand_descrete(probs.begin(), probs.end());
        newCentroids.emplace_back(centroids[c]);
    }
    return centroids;
}

template<typename Scalar>
double KMeans::point_cost(const std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> &centroids, const Eigen::RowVector<Scalar, Eigen::Dynamic> &point) {
    double minDist = (point - centroids[0]).squaredNorm();
    for (int i = 1; i < centroids.size(); i++) {
        double dist = (point - centroids[i]).squaredNorm();
        if (dist < minDist) {
            minDist = dist;
        }
    }
    return minDist;
}

template<typename Scalar>
std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> KMeans::fit(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data) {
    int n = data.rows();
    int dim = data.cols();

    // KMeansⅡ初始化质心
    // auto start = std::chrono::system_clock::now();

    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> centroids = init_centroids<Scalar>(data);
    k = centroids.size();

    // auto end = std::chrono::system_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << "init centroids cost " << double(duration.count()) << " ms" << std::endl;

    std::vector<std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>> clusters(k, std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>());
    std::vector<int> labels(n, 0);
    for (int iter = 0; iter < max_iterations; iter++) {
        // 清空clusters
        for (int i = 0; i < k; i++) {
            clusters[i].clear();
        }

        // 指派点到最近的质心
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            const auto &point = data.row(i);
            Scalar minDist = (point - centroids[0]).squaredNorm();
            int minDistId = 0;
            for (int j = 1; j < k; j++) {
                Scalar dist = (point - centroids[j]).squaredNorm();
                if (dist < minDist) {
                    minDist = dist;
                    minDistId = j;
                }
            }
            labels[i] = minDistId;
        }

        // 构建簇
        for (int i = 0; i < n; i++) {
            clusters[labels[i]].emplace_back(data.row(i));
        }

        // 更新每个簇的质心
        std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> oldCentroids = centroids;
        #pragma omp parallel for
        for (int i = 0; i < k; i++) {
            // 避免空簇
            if (clusters[i].empty()) {
                // 将空簇的质心重新初始化为所有点中距离其它质心最远的点
                int maxDistId = 0;
                Scalar maxDist = 0;
                for (int j = 0; j < n; j++) {
                    Scalar dist = point_cost<Scalar>(oldCentroids, data.row(j));
                    if (dist > maxDist) {
                        maxDist = dist;
                        maxDistId = j;
                    }
                }
                centroids[i] = data.row(maxDistId);
                continue;
            }
            // 计算clusters[i]中所有点的均值
            Eigen::RowVector<Scalar, Eigen::Dynamic> sum(dim);
            sum.setZero();
            for (const auto &point : clusters[i]) {
                sum += point;
            }
            centroids[i] = sum / clusters[i].size();
        }

        bool converged = true;
        // 检查质心是否变化
        for (int i = 0; i < k; i++) {
            // 1e-4：scikit-learn中的标准
            if ((centroids[i] - oldCentroids[i]).squaredNorm() > 1e-4) {
                converged = false;
                break;
            }
        }
        if (converged) {
            break;
        }
    }
    return centroids;
}

#endif // __KMEANS_HPP__