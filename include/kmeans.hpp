#ifndef __KMEANS_HPP__
#define __KMEANS_HPP__
#include <eigen-3.4.0/Eigen/Dense>
#ifndef EIGEN_USE_MKL_ALL
#define EIGEN_USE_MKL_ALL
#endif

#ifndef EIGEN_VECTORIZE_SSE4_2
#define EIGEN_VECTORIZE_SSE4_2
#endif
#include <vector>
#include <omp.h>
#include "random.hpp"
#include "utils.hpp"
#include "spdlog_common.h"

class KMeans {
public:
    KMeans(int k, std::string initMode, int maxIter = 20, int initSteps = 2, double tol = 1e-4) : k(k), maxIter(maxIter), initSteps(initSteps), tol(tol), initMode(initMode) {}

    void set_k(int k) { 
        this->k = k; 
    }

    /**
     * @brief Random初始化质心
     * @param data: 数据集
     * @return: 初始化后的质心
    */
    template<typename Scalar>
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> random_init_centroids(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data);

    /**
     * @brief KMeans++初始化质心
     * @param data: 数据集
     * @return: 初始化后的质心
    */
    template<typename Scalar>
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> kmeans_plus_plus_init_centroids(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data);

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
    double tol;
    // the way to init centroids
    std::string initMode;
    // random generator
    Random random;
};

template<typename Scalar>
std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> KMeans::random_init_centroids(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data) {
    int n = data.rows();
    int dim = data.cols();
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> centroids;
    for (int i = 0; i < k; i++) {
        centroids.emplace_back(data.row(random.randint(0, n - 1)));
    }
    return centroids;
}

template<typename Scalar>
std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> KMeans::kmeans_plus_plus_init_centroids(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data) {
    int n = data.rows();
    int dim = data.cols();
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> centroids;
    centroids.emplace_back(data.row(random.randint(0, n - 1)));
    for (int i = 1; i < k; i++) {
        std::vector<Scalar> minDists(n, 0);
        Scalar sum = 0;
        #pragma omp parallel for reduction(+:sum)
        for (int j = 0; j < n; j++) {
            Scalar minDist = point_cost<Scalar>(centroids, data.row(j));
            minDists[j] = minDist * minDist;
            sum += minDists[j];
        }
        Scalar r = static_cast<Scalar>(random.randn());
        Scalar prob = r * sum;
        int j = 0;
        while (prob > minDists[j]) {
            prob -= minDists[j];
            j++;
        }
        centroids.emplace_back(data.row(j));
    }
    return centroids;
}

template<typename Scalar>
std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> KMeans::init_centroids(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data) {
    int n = data.rows();
    int dim = data.cols();

    // 随机选取第一个质心
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> centroids;
    int c0 = random.randint(0, n - 1);
    centroids.emplace_back(data.row(c0));

    // KMeansⅡ选取方法
    // initSteps为超参数，一般取lg(phi), spark默认取2， l=2k
    // 如果超过100万个数据点，且k较大，取initSteps=5，避免第一轮取的质心小于k
    if (n > 1000000 && k > 20) {
        initSteps = 5;
    }

    const int l = 2 * k;
    for (int round = 0; round < initSteps; round++) {
        // KITSUNE可能会溢出，简单处理下，如果dim=115, 则使用double
        if (dim == 115) {
            std::vector<double> minDists(n, 0);
            double sum = 0;
            #pragma omp parallel for reduction(+:sum)
            for (int i = 0; i < n; i++) {
                double minDist = point_cost<Scalar>(centroids, data.row(i));
                minDists[i] = minDist * minDist;
                sum += minDists[i];
            }
            // 加权概率选取质心
            #pragma omp parallel for
            for (int i = 0; i < n; i++) {
                double prob = l * minDists[i] / sum;
                #pragma omp critical
                {
                    if (random.randn() < prob) {
                        centroids.emplace_back(data.row(i));
                    }
                }
            }
            continue;
        }
        std::vector<Scalar> minDists(n, 0);
        Scalar sum = 0;
        #pragma omp parallel for reduction(+:sum)
        for (int i = 0; i < n; i++) {
            Scalar minDist = point_cost<Scalar>(centroids, data.row(i));
            minDists[i] = minDist * minDist;
            sum += minDists[i];
        }
        // 加权概率选取质心
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            Scalar prob = l * minDists[i] / sum;
            Scalar r = static_cast<Scalar>(random.randn());
            #pragma omp critical
            {
                if (r < prob) {
                    centroids.emplace_back(data.row(i));
                }
            }
        }
    }

    // 可能大于k个质心，继续后续处理，先计算出权重：表示距离C中x点最近的点的个数
    std::vector<int> weights(centroids.size(), 0);
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        int minIndex = 0;
        Scalar minDist = utils::euclidean_distance<Scalar>(data.row(i), centroids[0]);
        for (int j = 1; j < centroids.size(); j++) {
            Scalar dist = utils::euclidean_distance<Scalar>(data.row(i), centroids[j]);
            if (dist < minDist) {
                minDist = dist;
                minIndex = j;
            }
        }
        #pragma omp atomic
        weights[minIndex]++;
    }

    // recluster过程：根据weights权重调用K-Means++选取k个质心
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> newCentroids(k, Eigen::RowVector<Scalar, Eigen::Dynamic>(dim));
    // 先选取一个质心
    c0 = random.rand_discrete(weights.begin(), weights.end());
    newCentroids[0] = centroids[c0];

    // 根据距离和权重选择其余的质心
    for (int i = 1; i < k; i++) {
        std::vector<Scalar> minDists(centroids.size(), 0);
        // 计算每个候选质心到已选质心集合的最小距离平方
        Scalar sum = 0;
        #pragma omp parallel for reduction(+:sum)
        for (int j = 0; j < centroids.size(); j++) {
            Scalar minDist = point_cost<Scalar>(newCentroids, centroids[j]);
            minDists[j] = minDist * minDist;
            sum += minDists[j];
        }
        // 计算每个候选质心被选为新质心的权重（最小距离平方 * 候选质心权重）
        std::vector<Scalar> probs(centroids.size(), 0);
        #pragma omp parallel for
        for (int j = 0; j < centroids.size(); j++) {
            probs[j] = minDists[j] * weights[j] / sum;
        }

        // 根据权重选择新的质心
        int c = random.rand_discrete(probs.begin(), probs.end());
        newCentroids[i] = centroids[c];
    }
    return newCentroids;
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
    
    // 初始化质心
    std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>> centroids;
    if (initMode == "kmeans||") {
        centroids = init_centroids<Scalar>(data);
    } else if (initMode == "kmeans++") {
        centroids = kmeans_plus_plus_init_centroids<Scalar>(data);
    } else if (initMode == "random") {
        centroids = random_init_centroids<Scalar>(data);
    } else {
        throw std::invalid_argument("initMode must be one of 'kmeans||', 'kmeans++' and 'random'");
    }

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
        #pragma omp parallel for
        for (int i = 0; i < k; i++) {
            // 避免空簇
            if (clusters[i].empty()) {
                // 选择离已有质心最远的点作为新质心
                int maxDistId = 0;
                Scalar maxDist = utils::euclidean_distance<Scalar>(oldCentroids[0], oldCentroids[0]);
                for (int j = 1; j < k; j++) {
                    Scalar dist = utils::euclidean_distance<Scalar>(oldCentroids[j], oldCentroids[j]);
                    if (dist > maxDist) {
                        maxDist = dist;
                        maxDistId = j;
                    }
                }
                centroids[i] = oldCentroids[maxDistId];
                continue;
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
            if (utils::euclidean_distance<Scalar>(oldCentroids[i], centroids[i]) > tol) {
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