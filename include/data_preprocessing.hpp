/**
 * @file data_preprocessing.hpp
 * @brief Data preprocessing functions
 * @date 2023-05-10
 * @version 0.0.1
 * @author cuterwrite
*/
#ifndef __DATA_PREPROCESSING_HPP__
#define __DATA_PREPROCESSING_HPP__
#include <eigen3/Eigen/Dense>
#include <vector>

class DataPreprocessor {
public:
    // 构造函数
    DataPreprocessor() = default;

    /**
     * 从文件中读取输入数据
     * 参数：
     *  - filename: 数据文件路径
     * 返回：读取的数据
    */
    Eigen::MatrixXd read_input_data(const std::string& filename);

    /**
     * 选择一个较小的样本子集
     * 参数：
     *  - data: 输入数据
     * - subset_size: 子集大小
     * 返回：选定的子集
    */
    Eigen::MatrixXd select_subset(const Eigen::MatrixXd& data, unsigned int subset_size);

    /**
     * 计算特征矩阵的协方差矩阵
     * 参数：
     *  - data： 输入数据
     * 返回：协方差矩阵
    */
    Eigen::MatrixXd compute_covariance_matrix(const Eigen::MatrixXd& data);

    /**
     * 使用PCA方法降维
     * 参数： 
     *  - data: 输入数据
     *  - num_components: 要保留的主成分数量
     * 返回：
     *   降维后的数据
    */
    Eigen::MatrixXd reduce_dimensionality(const Eigen::MatrixXd& data, unsigned int num_components);
    
    /**
     * 对数据进行K-means聚类并筛选可能的簇个数
     * 参数：
     *   - data: 输入数据
     *   - min_clusters: 最小聚类数
     *   - max_clusters: 最大聚类数
     * 返回：
     *   筛选出的可能的簇个数
    */
    std::vector<unsigned int> estimate_num_clusters(const Eigen::MatrixXd& data, unsigned int min_clusters, unsigned int max_clusters);
};

Eigen::MatrixXd DataPreprocessor::reduce_dimensionality(const Eigen::MatrixXd& data, unsigned int num_components) {
    // @TODO
    return Eigen::MatrixXd();
}

std::vector<unsigned int> DataPreprocessor::estimate_num_clusters(const Eigen::MatrixXd& data, unsigned int min_clusters, unsigned int max_clusters) {
    // @TODO
    return std::vector<unsigned int>();
}

Eigen::MatrixXd DataPreprocessor::read_input_data(const std::string& filename) {
    // @TODO
    return Eigen::MatrixXd();
}

Eigen::MatrixXd DataPreprocessor::select_subset(const Eigen::MatrixXd& data, unsigned int subset_size) {
    // @TODO
    return Eigen::MatrixXd();
}

Eigen::MatrixXd DataPreprocessor::compute_covariance_matrix(const Eigen::MatrixXd& data) {
    // @TODO
    return Eigen::MatrixXd();
}

#endif // __DATA_PREPROCESSING_HPP__