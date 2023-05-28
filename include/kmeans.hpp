#ifndef __KMEANS_HPP__
#define __KMEANS_HPP__
#include "eigen-3.4.0/Eigen/Dense"
#include <vector>
#include <omp.h>
#include "random.hpp"
#include "utils.hpp"
#include "spdlog_common.h"

class KMeans {
public:
    KMeans(int k, int maxIter = 20, int initSteps = 2, float tol = 0.0001) : k(k), maxIter(maxIter), initSteps(initSteps), tol(tol) {}
    void set_k(int k) { 
        this->k = k; 
    }

    /**
     * @brief KMeans||初始化质心
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
    Scalar point_cost(const std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> &centroids, const Eigen::RowVector<Scalar, Eigen::Dynamic> &point);

    /**
     * @brief KMeans聚类
     * @param data: 数据集
     * @return: 聚类后的质心
    */
    template<typename Scalar>
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> fit(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data);

private:
    // The number of clusters to create.
    int k;
    // max number of iterations (>= 0)
    int maxIter;
    // The number of steps for k-means|| initialization mode. Must be > 0.
    int initSteps;
    // the convergence tolerance for iterative algorithms (>= 0).
    float tol;
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
    // initSteps为超参数，一般取lgn, spark默认取2， l=2k
    const int l = 2 * k;
    for (int round = 0; round < initSteps; round++) {
        std::vector<Scalar> minDists(n, 0);
        // Scalar sum = 0; 无论float还是double都用double存和，因为KITSUNE可能会溢出
        double sum = 0;
        #pragma omp parallel for reduction(+:sum)
        for (int i = 0; i < n; i++) {
            minDists[i] = point_cost<Scalar>(centroids, data.row(i));
            sum += minDists[i];
        }
        // 加权概率选取质心
        for (int i = 0; i < n; i++) {
            Scalar prob = l * minDists[i] / sum;
            if (random.randn() < prob) {
                centroids.emplace_back(data.row(i));
            }
        }
    }

    // 候选质心数小于k，则返回所有质心
    if (centroids.size() <= k) {
        // SPDLOG_INFO("centroids size: {} <= {}", centroids.size(), k);
        return centroids;
    }

    // 可能大于k个质心，继续后续处理，先计算出权重：表示距离x点最近的点的个数
    std::vector<int> weights(centroids.size(), 0);
    for (int i = 0; i < centroids.size(); i++) {
        int minIndex = 0;
        Scalar minDist = utils::euclidean_distance<Scalar>(centroids[i], centroids[0]);
        for (int j = 1; j < centroids.size(); j++) {
            Scalar dist = utils::euclidean_distance<Scalar>(centroids[i], centroids[j]);
            if (dist < minDist) {
                minDist = dist;
                minIndex = j;
            }
        }
        weights[minIndex]++;
    }

    // recluster过程：根据weights权重选取k个质心
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> newCentroids(k, Eigen::RowVector<Scalar, Eigen::Dynamic>(dim));
    // 先选取一个质心
    c0 = random.rand_descrete(weights.begin(), weights.end());
    newCentroids[0] = centroids[c0];

    // 根据距离和权重选择其余的质心
    for (int i = 1; i < k; i++) {
        std::vector<Scalar> minDists(centroids.size(), 0);
        // 计算每个候选质心到已选质心集合的最小距离
        for (int j = 0; j < centroids.size(); j++) {
            minDists[j] = point_cost<Scalar>(newCentroids, centroids[j]);
        }
        // 计算每个候选质心被选为新质心的权重（距离 * 候选质心权重）
        std::vector<Scalar> probs(centroids.size(), 0);
        for (int j = 0; j < centroids.size(); j++) {
            probs[j] = minDists[j] * weights[j];
        }

        // 根据权重选择新的质心
        int c = random.rand_descrete(probs.begin(), probs.end());
        newCentroids[i] = centroids[c];
    }
    return centroids;
}

template<typename Scalar>
Scalar KMeans::point_cost(const std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> &centroids, const Eigen::RowVector<Scalar, Eigen::Dynamic> &point) {
    Scalar minDist = utils::euclidean_distance<Scalar>(point, centroids[0]);
    for (int i = 1; i < centroids.size(); i++) {
        Scalar dist = utils::euclidean_distance<Scalar>(point, centroids[i]);
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
    auto centroids = init_centroids<Scalar>(data);
    k = centroids.size();

    std::vector<std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>> clusters(k, std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>());
    for (int iter = 0; iter < maxIter; iter++) {
        // 清空clusters
        for (int i = 0; i < k; i++) {
            clusters[i].clear();
        }

        // 指派点到最近的质心
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            const auto &point = data.row(i);
            Scalar minDist = utils::euclidean_distance<Scalar>(point, centroids[0]);
            int minDistId = 0;
            for (int j = 1; j < k; j++) {
                Scalar dist = utils::euclidean_distance<Scalar>(point, centroids[j]);
                if (dist < minDist) {
                    minDist = dist;
                    minDistId = j;
                }
            }
            #pragma omp critical
            clusters[minDistId].emplace_back(point);
        }

        // 更新每个簇的质心
        std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> oldCentroids = centroids;
        for (int i = 0; i < k; i++) {
            // 避免空簇
            if (clusters[i].empty()) {
                // 随机选择其它非空簇的质心作为新质心
                // todo
                // continue;
            }
            // 计算clusters[i]中所有点的均值
            Eigen::RowVector<Scalar, Eigen::Dynamic> sum = Eigen::RowVector<Scalar, Eigen::Dynamic>::Zero(dim);
            for (const auto &point : clusters[i]) {
                sum += point;
            }
            centroids[i] = sum / clusters[i].size();
        }

        bool converged = true;
        // 检查质心是否变化
        for (int i = 0; i < k; i++) {
            // 1e-4：scikit-learn中的标准
            if (utils::euclidean_distance<Scalar>(oldCentroids[i], centroids[i]) > 1e-4) {
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