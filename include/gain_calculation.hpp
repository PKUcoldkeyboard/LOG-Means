#ifndef __GAIN_CALCULATION_HPP__
#define __GAIN_CALCULATION_HPP__

#include <eigen3/Eigen/Dense>
#include <vector>

class GainCalculator {
public:
    // 构造函数
    GainCalculator() = default;

    /**
     * 通过取不同的初始聚类中心，得到多个聚类结果
     * 参数：
     *  - data: 输入数据
     *  - num_clusters: 簇的数量
     *  - num_init: 不同初始化的次数
     * 返回：多个聚类结果（每个聚类结果包含每个数据点的簇标签）
     */
    std::vector<std::vector<int>> multiple_k_means_init(const Eigen::MatrixXd& data, unsigned int num_clusters, unsigned int num_init);

    /**
     * 针对每个聚类结果，计算其聚类中心的Gain值
     * 参数：
     *  - data: 输入数据
     *  - cluster_results: 多个聚类结果
     * 返回：每个聚类结果的Gain值
     */
    std::vector<double> compute_gain_values(const Eigen::MatrixXd& data, const std::vector<std::vector<int>>& cluster_results);

    /**
     * 计算Sum of Squares（SS）参数
     * 参数：
     *  - data: 输入数据
     *  - cluster_result: 单个聚类结果
     * 返回：计算出的SS值
     */
    double compute_sum_of_squares(const Eigen::MatrixXd& data, const std::vector<int>& cluster_result);

    /**
     * 计算Degrees of Freedom（Df）参数
     * 参数：
     *  - data: 输入数据
     *  - cluster_result: 单个聚类结果
     * 返回：计算出的Df值
     */
    double compute_degrees_of_freedom(const Eigen::MatrixXd& data, const std::vector<int>& cluster_result);
};

std::vector<std::vector<int>> GainCalculator::multiple_k_means_init(const Eigen::MatrixXd& data, unsigned int num_clusters, unsigned int num_init) {
    // @TODO
    return std::vector<std::vector<int>>();
}

std::vector<double> GainCalculator::compute_gain_values(const Eigen::MatrixXd& data, const std::vector<std::vector<int>>& cluster_results) {
    // @TODO
    return std::vector<double>();
}

double GainCalculator::compute_sum_of_squares(const Eigen::MatrixXd& data, const std::vector<int>& cluster_result) {
    // @TODO
    return 0.0;
}

#endif // __GAIN_CALCULATION_HPP__