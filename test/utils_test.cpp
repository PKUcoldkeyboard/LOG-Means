#include <gtest/gtest.h>
#include "utils.hpp"

class UtilsTest : public ::testing::Test {
protected:

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

TEST_F(UtilsTest, TestEuclideanDistance) {
    // 在此处编写针对计算欧氏距离的测试代码

    // 1. 测试两个相同的点之间的距离
    Eigen::VectorXd point1(2);
    Eigen::VectorXd point2(2);
    point1 << 1.0, 2.0;
    point2 << 1.0, 2.0;
    EXPECT_DOUBLE_EQ(utils::euclidean_distance(point1, point2), 0.0);

    // 2. 测试两个不同的点之间的距离
    // 清空
    Eigen::VectorXd point3(2);
    Eigen::VectorXd point4(2);
    point3 << 1.0, 2.0;
    point4 << 3.0, 4.0;
    EXPECT_DOUBLE_EQ(utils::euclidean_distance(point3, point4), 2.8284271247461903);

    Eigen::VectorXd point5(3);
    Eigen::VectorXd point6(3);
    point5 << 1.0, 2.0, 3.0;
    point6 << 4.0, 5.0, 6.0;
    EXPECT_DOUBLE_EQ(utils::euclidean_distance(point5, point6), 5.196152422706632);

    Eigen::VectorXd point7(4);
    Eigen::VectorXd point8(4);
    point7 << 1.0, 2.0, 3.0, 4.0;
    point8 << 5.0, 6.0, 7.0, 8.0;
    EXPECT_DOUBLE_EQ(utils::euclidean_distance(point7, point8), 8.0);
}

TEST_F(UtilsTest, TestComputeSSE1) {
    // 在此处编写针对计算SSE的测试代码

    // 1. 测试只有一个聚类结果的情况
    std::vector<Cluster> clusters;
    Eigen::VectorXd center(2);
    center << 1.0, 2.0;
    std::vector<Eigen::VectorXd> points;
    points.push_back(center);
    points.push_back(center);
    points.push_back(center);
    clusters.push_back(std::make_pair(center, points));
    EXPECT_DOUBLE_EQ(utils::compute_sse(clusters), 0.0);
}

TEST_F(UtilsTest, TestComputeSSE2) {
    // 在此处编写针对计算SSE的测试代码

    // 2. 测试有多个聚类结果的情况
    std::vector<Cluster> clusters;
    Eigen::VectorXd center1(2);
    center1 << 1.0, 2.0;
    std::vector<Eigen::VectorXd> points1;
    points1.push_back(center1);
    points1.push_back(center1);
    points1.push_back(center1);
    clusters.push_back(std::make_pair(center1, points1));

    Eigen::VectorXd center2(2);
    center2 << 3.0, 4.0;
    std::vector<Eigen::VectorXd> points2;
    points2.push_back(center2);
    points2.push_back(center2);
    points2.push_back(center2);
    clusters.push_back(std::make_pair(center2, points2));

    EXPECT_DOUBLE_EQ(utils::compute_sse(clusters), 0.0);
}

TEST_F(UtilsTest, TestComputeSSE3) {
    // 在此处编写针对计算SSE的测试代码

    // 3. 测试有多个聚类结果的情况
    std::vector<Cluster> clusters;
    Eigen::VectorXd center1(2);
    center1 << 1.0, 2.0;
    std::vector<Eigen::VectorXd> points1;
    points1.push_back(center1);
    points1.push_back(center1);
    points1.push_back(center1);
    clusters.push_back(std::make_pair(center1, points1));

    Eigen::VectorXd center2(2);
    center2 << 3.0, 4.0;
    std::vector<Eigen::VectorXd> points2;
    points2.push_back(center2);
    points2.push_back(center2);
    points2.push_back(center2);
    clusters.push_back(std::make_pair(center2, points2));

    Eigen::VectorXd center3(2);
    center3 << 5.0, 6.0;
    std::vector<Eigen::VectorXd> points3;
    points3.push_back(center3);
    points3.push_back(center3);
    points3.push_back(center3);
    clusters.push_back(std::make_pair(center3, points3));

    EXPECT_DOUBLE_EQ(utils::compute_sse(clusters), 0.0);
}

TEST_F(UtilsTest, TestComputeSSE4) {
    // 3. 测试聚类结果为空的情况
    std::vector<Cluster> clusters;
    EXPECT_DOUBLE_EQ(utils::compute_sse(clusters), 0.0);
}

TEST_F(UtilsTest, TestComputeSSE5) {
    // 4. 测试聚类结果中有空聚类的情况
    std::vector<Cluster> clusters;
    Eigen::VectorXd center(2);
    center << 1.0, 2.0;
    std::vector<Eigen::VectorXd> points;
    points.push_back(center);
    points.push_back(center);
    points.push_back(center);
    clusters.push_back(std::make_pair(center, points));
    clusters.push_back(std::make_pair(center, points));
    clusters.push_back(std::make_pair(center, points));
    clusters.push_back(std::make_pair(center, points));
    EXPECT_DOUBLE_EQ(utils::compute_sse(clusters), 0.0);
}
