#include <gtest/gtest.h>
#include <eigen3/Eigen/Dense>
#include "gain_calculation.hpp"

class GainCalculationTest : public ::testing::Test {
protected:
    // 在此处初始化测试用的对象
    GainCalculator gc;

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

TEST_F(GainCalculationTest, TestMultipleClusterResults) {
    // 在此处编写针对多个聚类结果的测试代码
}

TEST_F(GainCalculationTest, TestComputeGainValues) {
    // 在此处编写针对计算聚类中心Gain值的测试代码
}

TEST_F(GainCalculationTest, TestComputeSumOfSquaresAndDegreesOfFreedom) {
    // 在此处编写针对计算Sum of Squares（SS）和Degrees of Freedom（Df）参数的测试代码
}