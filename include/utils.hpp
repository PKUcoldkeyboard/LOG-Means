#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <fstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <omp.h>
#include <sstream>
#include <eigen3/Eigen/Dense>
#include "spdlog_common.h"

namespace utils {
    /**
     * 从文件中读取数据并存储为Eigen矩阵
     * 参数：
     *  - location: 数据集文件路径
     *  - num: 数据集中的数据数量
     *  - dim: 数据集中的数据维度
     *  - classes: 数据集中的类别数量
     * 返回：从文件中读取的数据矩阵
     */
    template<typename Scalar>
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> read_data_from_file(std::string location, int num, int dim);

    /**
     * 计算两点之间的欧氏距离
     * 参数：
     *  - point1: 点1
     *  - point2: 点2
     * 返回：两点之间的欧氏距离
     */
    template<typename Scalar>
    Scalar euclidean_distance(const Eigen::RowVector<Scalar, Eigen::Dynamic>& point1, const Eigen::RowVector<Scalar, Eigen::Dynamic>& point2);
    /**
     * 计算聚类结果的SSE
     * 参数：
     * - clusters: 聚类结果
     * 返回：SSE
     *
    */
    template<typename Scalar>
    Scalar compute_sse(const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>& data, const std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>& centroids);
    /**
     * 计算算法估计类与真实类数量的误差
     * 参数：
     * - k: 算法估计类
     * - c: 真实类数量
    */
    double compute_delta(int k, int c);

} // namespace utils

template<typename Scalar>
Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> utils::read_data_from_file(std::string location, int num, int dim) {
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> data(num, dim);
    // 打开文件
    int fd = open(location.c_str(), O_RDONLY);
    if (fd == -1) {
        SPDLOG_ERROR("Failed to open file {}", location);
        exit(1);
    }

    // 获取文件大小
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        SPDLOG_ERROR("Failed to get file size");
        exit(1);
    }

    // mmap读取文件
    char *addr = static_cast<char*>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    if (addr == MAP_FAILED) {
        SPDLOG_ERROR("Failed to mmap file {}", location);
        close(fd);
        exit(1);
    }

    // 遍历每一行
    std::istringstream iss(addr);
    for (int i = 0; i < num; i++) {
        std::string line;
        if (!std::getline(iss, line)) {
            SPDLOG_ERROR("Failed to read line {}", i);
            exit(1);
        }
        // 解析每一行的数据
        std::istringstream iss_line(line);
        for (int j = 0; j < dim; j++) {
            std::string val;
            if (!std::getline(iss_line, val, ',')) {
                SPDLOG_ERROR("Failed to read value ({}, {})", i, j);
                exit(1);
            }
            // 将数据转换为浮点数并存储到 Eigen 矩阵中
            try {
                if constexpr (std::is_same<Scalar, float>::value) {
                    data(i, j) = std::stof(val);
                } else if constexpr (std::is_same<Scalar, double>::value) {
                    data(i, j) = std::stod(val);
                } else {
                    SPDLOG_ERROR("Unsupported data type");
                    exit(1);
                }
            } catch (std::invalid_argument& e) {
                SPDLOG_ERROR("Failed to convert value ({}, {}, {}) to float due to invalid argument error", i, j, val);
                exit(1);
            } catch (std::out_of_range& e) {
                // 存储的数据超出了浮点数的范围，视为0
                if constexpr (std::is_same<Scalar, float>::value) {
                    data(i, j) = 0.0f;
                } else if constexpr (std::is_same<Scalar, double>::value) {
                    data(i, j) = 0.0;
                }
                continue;
            }
        }
    }

    // 释放内存映射
    if (munmap(addr, sb.st_size) == -1) {
        SPDLOG_ERROR("Failed to munmap file");
        exit(1);
    }
    close(fd);

    return data;
}

template<typename Scalar>
Scalar utils::euclidean_distance(const Eigen::RowVector<Scalar, Eigen::Dynamic>& point1, const Eigen::RowVector<Scalar, Eigen::Dynamic>& point2) {
    return (point1 - point2).norm();
}

template<typename Scalar>
Scalar utils::compute_sse(const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic>& data, const std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>& centroids) {
    Scalar sse = 0.0;
    int n = data.rows();

    #pragma omp parallel for reduction(+:sse)
    for (int i = 0; i < n; i++) {
        Scalar minDist = euclidean_distance<Scalar>(data.row(i), centroids[0]);
        for (int j = 1; j < centroids.size(); j++) {
            Scalar dist = euclidean_distance(data.row(i), centroids[j]);
            if (dist < minDist) {
                minDist = dist;
            }
        }
        sse += minDist;
    }
    return sse;
}

double utils::compute_delta(int k, int c) {
    return std::abs(k - c) / static_cast<double>(c);
}

#endif // __UTILS_HPP__