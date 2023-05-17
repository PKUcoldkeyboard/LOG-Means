#ifndef __KMEANS_HPP__
#define __KMEANS_HPP__
#include <eigen3/Eigen/Dense>
#include <vector>
#include <omp.h>
#include "random.hpp"

namespace cluster {

    double euclidean_distance(Eigen::VectorXd &point1, Eigen::VectorXd &point2);

    // K-means clustering algorithm
    std::vector<Eigen::VectorXd> kmeans(std::vector<Eigen::VectorXd> &data, int k, int max_iterations = 1000);

}

double cluster::euclidean_distance(Eigen::VectorXd &point1, Eigen::VectorXd &point2) {
    return (point1 - point2).norm();
}

std::vector<Eigen::VectorXd> cluster::kmeans(std::vector<Eigen::VectorXd> &data, int k, int max_iterations) {
    // 初始化centroids，使用随机选择的k个点
    std::vector<Eigen::VectorXd> centroids(k);
    Random random;
    for (int i = 0; i < k; ++i) {
        centroids[i] = data[random.randint(0, data.size() - 1)];
    }

    int dimensions = data[0].size();

    // 迭代直至收敛
    for (int iteration = 0; iteration < max_iterations; ++iteration) {
        std::vector<std::vector<Eigen::VectorXd>> clusters(k);
        #pragma omp parallel for
        for (int j = 0; j < data.size(); ++j) {
            Eigen::VectorXd &point = data[j];
            double minDist = std::numeric_limits<double>::max();
            int minIndex = 0;
            for (int i = 0; i < k; ++i) {
                double dist = euclidean_distance(point, centroids[i]);
                if (dist < minDist) {
                    minDist = dist;
                    minIndex = i;
                }
            }
            #pragma omp critical
            clusters[minIndex].push_back(point);
        }

        std::vector<Eigen::VectorXd> new_centroids(k);
        #pragma omp parallel for
        for (int i = 0; i < k; ++i) {
            Eigen::VectorXd sum = Eigen::VectorXd::Zero(dimensions);
            for (const Eigen::VectorXd &point : clusters[i]) {
                sum += point;
            }
            new_centroids[i] = sum / clusters[i].size();
        }

        bool converged = true;
        for (int i = 0; i < k; ++i) {
            if (euclidean_distance(centroids[i], new_centroids[i]) > 1e-6) {
                converged = false;
                break;
            }
        }

        if (converged) {
            break;
        }
        centroids = new_centroids;
    }

    return centroids;
}

#endif // __KMEANS_HPP__