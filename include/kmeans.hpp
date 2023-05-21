#ifndef __KMEANS_HPP__
#define __KMEANS_HPP__
#include <eigen3/Eigen/Dense>
#include <vector>
#include <omp.h>
#include "random.hpp"
#include "utils.hpp"
#include "spdlog_common.h"

// 定义簇类型
typedef std::pair<Eigen::VectorXd, std::vector<Eigen::VectorXd>> Cluster;

class KMeans {
public:
    KMeans(int k, int max_iterations = 1000) : k(k), max_iterations(max_iterations) {}
    void set_k(int k) { 
        this->k = k; 
    }
    /**
     * 聚类
     * 参数：
     * - data: 数据集
     * 返回：聚类结果
    */
    std::pair<std::vector<Cluster>, std::vector<int>> fit(std::vector<Eigen::VectorXd> &data) {
        SPDLOG_INFO("Starting KMeans clustering, cluster number: {}, max iterations: {}", k, max_iterations);
        int n = data.size();
        int dim = data[0].size();

        // KMeans++初始化质心
        std::vector<Eigen::VectorXd> centroids;
        Random random;
        centroids.emplace_back(data[random.randint(0, n - 1)]);
        for (int i = 1; i < k; i++) {
            // 计算所有点到最近质心的距离平方
            std::vector<double> dists(n, 0);
            for (int j = 0; j < n; j++) {
                double minDist = std::numeric_limits<double>::max();
                for (int l = 0; l < i; l++) {
                    double dist = utils::euclidean_distance(data[j], centroids[l]);
                    if (dist < minDist) {
                        minDist = dist;
                    }
                }
                dists[j] = minDist * minDist;
            }
            // 根据距离平方作为权重随机选取下一个质心
            int index = random.rand_descrete(dists.begin(), dists.end());
            centroids.emplace_back(data[index]);
        }

        SPDLOG_INFO("Done random init centroids");

        // 初始化标签向量
        std::vector<int> labels(n, 0);
        // 初始化簇
        std::vector<std::vector<Eigen::VectorXd>> clusters(k, std::vector<Eigen::VectorXd>());

        for (int iter = 0; iter < max_iterations; iter++) {
            // 清空簇
            clusters.clear();
            clusters.resize(k, std::vector<Eigen::VectorXd>());

            // 对每个点进行归类
            #pragma omp parallel for
            for (int j = 0; j < n; j++) {
                auto &point = data[j];
                double minDist = std::numeric_limits<double>::max();
                int minIndex = 0;
                // 寻找距离最近的质心
                for (int i = 0; i < k; i++) {
                    double dist = utils::euclidean_distance(point, centroids[i]);
                    if (dist < minDist) {
                        minDist = dist;
                        minIndex = i;
                    }
                }
                // 添加点到最近的簇
                #pragma omp critical
                clusters[minIndex].push_back(point);
                // 保存标签
                labels[j] = minIndex;
            }
            
            // 更新每个簇的质心
            #pragma omp parallel for
            for (int i = 0; i < k; i++) {
                // 避免空簇问题
                if (!clusters[i].empty()) {
                    Eigen::VectorXd sum = Eigen::VectorXd::Zero(dim);
                    for(auto &point : clusters[i]) {
                        sum += point;
                    }
                    centroids[i] = sum / clusters[i].size();
                }
            }

            bool converged = true;
            // 检查质心是否变化
            for (int i = 0; i < k; i++) {
                if (utils::euclidean_distance(centroids[i], clusters[i][0]) > 1e-6) {
                    converged = false;
                    break;
                }
            }
            if (converged) {
                SPDLOG_INFO("KMeans clustering converged after {} iterations", iter);
                break;
            }
        }
        
        // 准备最终返回的簇
        std::vector<Cluster> resultCluster(k);
        for (int i = 0; i < k; i++) {
            resultCluster[i] = std::make_pair(centroids[i], clusters[i]);
        }

        SPDLOG_INFO("Done KMeans clustering");

        return {resultCluster, labels};
    }

private:
    // cluster number
    int k;
    // max iterations
    int max_iterations;
};

#endif // __KMEANS_HPP__