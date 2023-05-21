#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include<string>
#include<eigen3/Eigen/Dense>
#pragma once
typedef std::pair<Eigen::VectorXd, std::vector<Eigen::VectorXd>> Cluster;

namespace utils {
    /**
     * 从文件中读取数据并存储为Eigen矩阵
     * 参数：
     *  - filename: 文件名
     * 返回：从文件中读取的数据矩阵
     */
    Eigen::MatrixXd read_data_from_file(const std::string& filename);

    /**
     * 将矩阵数据写入文件
     * 参数：
     *  - filename: 文件名
     *  - data: 要写入文件的数据矩阵
     */
    void write_data_to_file(const std::string& filename, const Eigen::MatrixXd& data);

    /**
     * 计算两点之间的欧氏距离
     * 参数：
     *  - point1: 点1
     *  - point2: 点2
     * 返回：两点之间的欧氏距离
     */
    double euclidean_distance(const Eigen::VectorXd& point1, const Eigen::VectorXd& point2);
    /**
     * 计算聚类结果的SSE
     * 参数：
     * - clusters: 聚类结果
     * 返回：SSE
     *
    */
    double compute_sse(const std::vector<Cluster>& clusters);
    /**
     * 计算算法估计类与真实类数量的误差
     * 参数：
     * - k: 算法估计类
     * - c: 真实类数量
    */
    double compute_delta(int k, int c);

} // namespace utils

Eigen::MatrixXd utils::read_data_from_file(const std::string& filenmae) {
    // @TODO
    return Eigen::MatrixXd();
}

void utils::write_data_to_file(const std::string& filename, const Eigen::MatrixXd& data) {
    // @TODO
}

double utils::euclidean_distance(const Eigen::VectorXd& point1, const Eigen::VectorXd& point2) {
    return (point1 - point2).norm();
}

double utils::compute_sse(const std::vector<Cluster>& clusters) {
    double sse = 0.0;
    for (const auto& cluster : clusters) {
        const auto& centroid = cluster.first;
        const auto& points = cluster.second;
        for (const auto& point : points) {
            sse += utils::euclidean_distance(centroid, point) * utils::euclidean_distance(centroid, point);
        }
    }
    return sse;
}

double utils::compute_delta(int k, int c) {
    return std::abs(k - c) / static_cast<double>(c);
}

#endif // __UTILS_HPP__