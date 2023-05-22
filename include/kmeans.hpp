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
    KMeans(int k, int max_iterations = 100) : k(k), max_iterations(max_iterations) {}
    void set_k(int k) { 
        this->k = k; 
    }

    /**
     * @brief KMeans++初始化质心
     * @param data: 数据集
     * @return: 初始化后的质心
    */
    template<typename Scalar>
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> init_centroids(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> &data);

    /**
     * @brief KMeans聚类
     * @param data: 数据集
     * @return: 聚类后的质心
    */
    template<typename Scalar>
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> fit(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> &data);

private:
    // cluster number
    int k;
    // max iterations
    int max_iterations;
};

template<typename Scalar>
std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> KMeans::init_centroids(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> &data) {
    int n = data.rows();
    int dim = data.cols();

    // 随机选取第一个质心
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> centroids;
    Random random;
    int c0 = random.randint(0, n - 1);
    centroids.emplace_back(data.row(c0));

    std::vector<Scalar> dists(n, 0);
    int cnt = 1;
    while (cnt < k) {
        // 计算所有点到已有质心的最短距离平方, 作为选择下一个质心的权重
        for (int i = 0; i < n; i++) {
            Scalar minDist = (data.row(i) - centroids[0]).squaredNorm();
            for (int j = 1; j < cnt; j++) {
                Scalar dist = (data.row(i) - centroids[j]).squaredNorm();
                if (dist < minDist) {
                    minDist = dist;
                }
            }
            dists[i] = minDist;
        }
        // 根据距离的平方权重随机选择新的质心
        int newCentroidId = random.rand_descrete(dists.begin(), dists.end());
        centroids.emplace_back(data.row(newCentroidId));
        cnt++;
    }

    return centroids;
}

template<typename Scalar>
std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> KMeans::fit(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> &data) {
    SPDLOG_INFO("Starting KMeans clustering, cluster number: {}, max iterations: {}", k, max_iterations);
    int n = data.rows();
    int dim = data.cols();

    // KMeans++初始化质心
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> centroids = init_centroids<Scalar>(data);
    std::vector<std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>> clusters(k, std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>());
    std::vector<int> labels(n, 0);
    for (int iter = 0; iter < max_iterations; iter++) {
        clusters.clear();
        clusters.resize(k, std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>());
        
        // 指派点到最近的质心
        #pragma omp parallel for num_threads(4)
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
            {
                clusters[minDistId].emplace_back(point);
                labels[i] = minDistId;
            }
        }
        // 更新每个簇的质心
        std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> oldCentroids = centroids;
        for (int i = 0; i < k; i++) {
            // 避免空簇
            if (clusters[i].empty()) {
                continue;
            }
            // 计算clusters[i]中所有点的均值
            centroids[i] = clusters[i][0];
            for (int j = 1; j < clusters[i].size(); j++) {
                centroids[i] += clusters[i][j];
            }
            centroids[i] /= clusters[i].size();
        }

        bool converged = true;
        // 检查质心是否变化
        for (int i = 0; i < k; i++) {
            if ((centroids[i] - oldCentroids[i]).norm() > 1e-4) {
                converged = false;
                break;
            }
        }
        if (converged) {
            SPDLOG_INFO("KMeans clustering converged at iteration {}", iter);
            break;
        }
    }

    SPDLOG_INFO("Finished KMeans clustering");
    return centroids;
}

#endif // __KMEANS_HPP__