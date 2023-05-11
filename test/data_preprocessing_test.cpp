#include <gtest/gtest.h>
#include <eigen3/Eigen/Dense>
#include "data_preprocessing.hpp"

class DataPreprocessingTest : public ::testing::Test {
protected:
    // 在此处初始化测试用的对象
    DataPreprocessor dp;

    // 在此处添加测试用的输入数据
    Eigen::MatrixXd test_data;
    
    // 在此处添加其他需要的成员变量

    void SetUp() override {
        // 在此处初始化测试数据和其他需要的资源
    }

    void TearDown() override {
        // 在此处清理测试使用的资源
    }
};

TEST_F(DataPreprocessingTest, TestReadInputData) {
    // 在此处编写针对read_input_data函数的测试代码
}

TEST_F(DataPreprocessingTest, TestSelectSubset) {
    // 在此处编写针对select_subset函数的测试代码
}

TEST_F(DataPreprocessingTest, TestComputeCovarianceMatrix) {
    // 在此处编写针对compute_covariance_matrix函数的测试代码
}

TEST_F(DataPreprocessingTest, TestReduceDimensionality) {
    // 在此处编写针对reduce_dimensionality函数的测试代码
}

TEST_F(DataPreprocessingTest, TestEstimateNumClusters) {
    // 在此处编写针对estimate_num_clusters函数的测试代码
}
