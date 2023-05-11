#include<gtest/gtest.h>
#include <eigen3/Eigen/Dense>
#include "cluster_analysis.hpp"

class ClusterAnalysisTest : public ::testing::Test {
protected:
    // 在此处初始化测试用的对象
    ClusterAnalyzer ca;

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

TEST_F(ClusterAnalysisTest, TestDetermineNumClusters) {
    // 在此处编写根据Gain值，采用BIC方法确定最可能的簇个数的测试代码
}

TEST_F(ClusterAnalysisTest, TestKMeansClustering) {
    // 在此处编写对整个数据集进行K-means聚类的测试代码
}

TEST_F(ClusterAnalysisTest, TestSilhouetteCoefficient) {
    // 在此处编写评估聚类结果的质量的测试代码
}
