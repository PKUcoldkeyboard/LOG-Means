#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <fstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <omp.h>
#include <sstream>
#include <unordered_map>
#include "eigen-3.4.0/Eigen/Dense"
#include "spdlog_common.h"

namespace utils {
    // look-up table, 1e-38到1e38
    const std::unordered_map<int, double> lut = {
        {-38, 1e-38}, {-37, 1e-37}, {-36, 1e-36}, {-35, 1e-35}, {-34, 1e-34}, {-33, 1e-33},
        {-32, 1e-32}, {-31, 1e-31}, {-30, 1e-30}, {-29, 1e-29}, {-28, 1e-28}, {-27, 1e-27},
        {-26, 1e-26}, {-25, 1e-25}, {-24, 1e-24}, {-23, 1e-23}, {-22, 1e-22}, {-21, 1e-21},
        {-20, 1e-20}, {-19, 1e-19}, {-18, 1e-18}, {-17, 1e-17}, {-16, 1e-16}, {-15, 1e-15},
        {-14, 1e-14}, {-13, 1e-13}, {-12, 1e-12}, {-11, 1e-11}, {-10, 1e-10}, {-9, 1e-9},
        {-8, 1e-8}, {-7, 1e-7}, {-6, 1e-6}, {-5, 1e-5}, {-4, 1e-4}, {-3, 1e-3}, {-2, 1e-2},
        {-1, 1e-1}, {0, 1.0}, {1, 1e1}, {2, 1e2}, {3, 1e3}, {4, 1e4}, {5, 1e5},
        {6, 1e6}, {7, 1e7}, {8, 1e8}, {9, 1e9}, {10, 1e10}, {11, 1e11}, {12, 1e12},
        {13, 1e13}, {14, 1e14}, {15, 1e15}, {16, 1e16}, {17, 1e17}, {18, 1e18},
        {19, 1e19}, {20, 1e20}, {21, 1e21}, {22, 1e22}, {23, 1e23}, {24, 1e24},
        {25, 1e25}, {26, 1e26}, {27, 1e27}, {28, 1e28}, {29, 1e29}, {30, 1e30},
        {31, 1e31}, {32, 1e32}, {33, 1e33}, {34, 1e34}, {35, 1e35}, {36, 1e36},
        {37, 1e37}, {38, 1e38}
    };
    
    template <typename T> void read(FILE *f, T &x);
    inline void read_float(FILE* f, float &v);
    inline void read_double(FILE* f, double &v);

    /**
     * @brief 从文件中读取数据并存储为Eigen矩阵
     * @param location 数据集文件路径
     * @param num 数据集中的数据数量
     * @param dim 数据集中的数据维度
     * @param classes 数据集中的类别数量
     * @return 从文件中读取的数据矩阵
     */
    template<typename Scalar>
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> read_data_from_file(std::string location, int num, int dim);

    /**
     * @brief 从文件中读取数据并存储为Eigen矩阵(mmap方式)
     * @param location 数据集文件路径
     * @param num 数据集中的数据数量
     * @param dim 数据集中的数据维度
     * @param classes 数据集中的类别数量
     * @return 从文件中读取的数据矩阵
    */
    template<typename Scalar>
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> mmap_read_data_from_file(std::string location, int num, int dim);

    /**
     * @brief 从文件中读取数据并存储为Eigen矩阵(ifstream方式)
     * @param location 数据集文件路径
     * @param num 数据集中的数据数量
     * @param dim 数据集中的数据维度
     * @param classes 数据集中的类别数量
     * @return 从文件中读取的数据矩阵
    */
    template<typename Scalar>
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> ifs_read_data_from_file(std::string location, int num, int dim);

    /**
     * @brief 计算两点之间的欧氏距离
     * @param point1: 点1
     * @praam: point2: 点2
     * @return：两点之间的欧氏距离
     */
    template<typename Scalar>
    Scalar euclidean_distance(const Eigen::RowVector<Scalar, Eigen::Dynamic>& point1, const Eigen::RowVector<Scalar, Eigen::Dynamic>& point2);
    /**
     * @brief 计算聚类结果的SSE
     * @param clusters: 聚类结果
     * @return：SSE
     *
    */
    template<typename Scalar>
    Scalar compute_sse(const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& data, const std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>& centroids);
    /**
     * @brief 计算算法估计类与真实类数量的误差
     * @param k: 算法估计类
     * @param c: 真实类数量
     * @return 误差
    */
    double compute_delta(int k, int c);

} // namespace utils

template <typename T> void utils::read(FILE *f, T &x) {
    x = 0;
    char c = getc(f);
    T sig = 1;
    for (; !isdigit(c); c = getc(f)) {
        if (c == '-') {
            sig = -1;
        }
    }
    for (; isdigit(c); c = getc(f)) {
        x = (x << 3) + (x << 1) + c - '0';
    }
    x *= sig;
}

inline void utils::read_float(FILE* f, float &v) {
    char c = getc(f);
    int x = 0, z = 0;
    int i = 0;
    int sig = 1, sigz = 1;
    for (; !isdigit(c); c = getc(f)) {
        if (c == '-') {
            sig = -1;
        }
    }
    for (; isdigit(c); c = getc(f)) {
        x = x * 10 + c - '0';
    }
    if (c == '.') {
        for (c = getc(f); isdigit(c); c = getc(f)) {
            if (i < 12) {
                x = x * 10 + c - '0';
                i++;
            }
        }
    }
    if (c == 'e') {
        c = getc(f);
        if (c == '-') {
            sigz = -1;
        } else if (c == '+') {
            sigz = 1;
        } else {
            z = z * 10 + c - '0';
        }
        for (c = getc(f); isdigit(c); c = getc(f)) {
            z = z * 10 + c - '0';
        }
        z *= sigz;
        if (z - i < -38) {
            v = 0;
            return;
        }
    }
    v = sig * x * lut.at(z - i);
}

inline void utils::read_double(FILE* f, double &v) {
    char c = getc(f);
    int x = 0, z = 0;
    int i = 0;
    int sig = 1, sigz = 1;
    for (; !isdigit(c); c = getc(f)) {
        if (c == '-') {
            sig = -1;
        }
    }
    for (; isdigit(c); c = getc(f)) {
        x = x * 10 + c - '0';
    }
    if (c == '.') {
        for (c = getc(f); isdigit(c); c = getc(f)) {
            if (i < 12) {
                x = x * 10 + c - '0';
                i++;
            }
        }
    }
    if (c == 'e') {
        c = getc(f);
        if (c == '-') {
            sigz = -1;
        } else if (c == '+') {
            sigz = 1;
        } else {
            z = z * 10 + c - '0';
        }
        for (c = getc(f); isdigit(c); c = getc(f)) {
            z = z * 10 + c - '0';
        }
        z *= sigz;
        if (z - i < -308) {
            v = 0;
            return;
        }
    }
    v = sig * x * std::pow(10, z - i);
}

template<typename Scalar>
Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> utils::read_data_from_file(std::string location, int num, int dim) {
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> data(num, dim);
    FILE* f = fopen(location.c_str(), "r");
    if (f == nullptr) {
        SPDLOG_ERROR("Cannot open file {}", location);
        exit(1);
    }

    for (int i = 0; i < num; i++) {
        for (int j = 0; j < dim; j++) {
            if constexpr (std::is_same_v<Scalar, float>) {
                read_float(f, data(i, j));
            } else if constexpr (std::is_same_v<Scalar, double>) {
                read_double(f, data(i, j));
            }
        }
    }

    fclose(f);

    return data;
}

template<typename Scalar>
Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> utils::mmap_read_data_from_file(std::string location, int num, int dim) {
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> data(num, dim);
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
Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> utils::ifs_read_data_from_file(std::string location, int num, int dim) {
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> data(num, dim);
    std::ifstream file(location);

    // 打开文件
    if (!file.is_open()) {
        SPDLOG_ERROR("Failed to open file {}", location);
        exit(1);
    }

    // 遍历每一行
    std::string line;
    for (int i = 0; i < num; i++) {
        if (!std::getline(file, line)) {
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
    return data;
}

template<typename Scalar>
Scalar utils::euclidean_distance(const Eigen::RowVector<Scalar, Eigen::Dynamic>& point1, const Eigen::RowVector<Scalar, Eigen::Dynamic>& point2) {
    return (point1 - point2).norm();
    // Spark mllib Kmeans距离优化，lowerBoundOfSqDist
    // return point1.norm() - point2.norm();
}

template<typename Scalar>
Scalar utils::compute_sse(const Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& data, const std::vector<Eigen::RowVector<Scalar, Eigen::Dynamic>>& centroids) {
    Scalar sse = 0.0;
    int n = data.rows();

    #pragma omp parallel for reduction(+:sse)
    for (int i = 0; i < n; i++) {
        Scalar minDist = (data.row(i) - centroids[0]).squaredNorm();
        for (int j = 1; j < centroids.size(); j++) {
            Scalar dist = (data.row(i) - centroids[j]).squaredNorm();
            if (dist < minDist) {
                minDist = dist;
            }
        }
        sse += minDist;
    }
    return sse;
}

double utils::compute_delta(int k, int c) {
    return static_cast<double>(k - c) / static_cast<double>(c);
}

#endif // __UTILS_HPP__