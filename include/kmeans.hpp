#ifndef __KMEANS_HPP__
#define __KMEANS_HPP__
#include <eigen3/Eigen/Dense>
#include <vector>
#include <omp.h>
#include "random.hpp"
#include "utils.hpp"

// 定义簇类型
typedef std::pair<Eigen::VectorXd, std::vector<Eigen::VectorXd>> Cluster;

class KMeans {
public:
    KMeans(int k, int max_iterations = 100) : k(k), max_iterations(max_iterations) {}
    /**
     * 聚类
     * 参数：
     * - data: 数据集
     * 返回：聚类结果
    */
    std::pair<std::vector<Cluster>, std::vector<int>> fit(std::vector<Eigen::VectorXd> &data) {
        int n = data.size();
        int dim = data[0].size();

        // 使用随机数初始化质心
        std::vector<Eigen::VectorXd> centroids;
        Random random;
        for (int i = 0; i < k; ++i) {
            centroids.emplace_back(data[random.randint(0, n - 1)]);
        }

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
                break;
            }
        }
        
        // 准备最终返回的簇
        std::vector<Cluster> resultCluster(k);
        for (int i = 0; i < k; i++) {
            resultCluster[i] = std::make_pair(centroids[i], clusters[i]);
        }

        return {resultCluster, labels};
    }

private:
    // cluster number
    int k;
    // max iterations
    int max_iterations;
};

#endif // __KMEANS_HPP__