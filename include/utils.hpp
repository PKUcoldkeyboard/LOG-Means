#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include<string>
#include<eigen3/Eigen/Dense>

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

#endif // __UTILS_HPP__