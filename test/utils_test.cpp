#include <gtest/gtest.h>
#include "random.hpp"
#include "utils.hpp"

class UtilsTest : public ::testing::Test {
protected:
    
    // 在此处添加测试用的输入数据
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> data;
    
    Random random;

    void SetUp() override {
        // 在此处初始化测试数据和其它需要的资源，默认读取Avila数据集
        data = utils::read_data_from_file<float>("datasets/Avila.txt", 10430, 10);
        random.get_engine().seed(42);
    }

    void TearDown() override {
        // 在此处清理测试使用的资源
    }
};

TEST_F(UtilsTest, TestReadDataFromFile1) {
    // 读取数据集
    auto data = utils::read_data_from_file<float>("datasets/Avila.txt", 10430, 10);
    ASSERT_EQ(data.rows(), 10430);
    ASSERT_EQ(data.cols(), 10);

    // 使用mmap方式读取数据集，随机对比5行验证数据是否正确
    auto data2 = utils::mmap_read_data_from_file<float>("datasets/Avila.txt", 10430, 10);

    // 随机生成5个不同的行号
    std::vector<int> rows;
    for (int i = 0; i < 5; i++) {
        rows.push_back(random.randint(0, 10430));
    }

    // 验证数据是否正确
    for (int i = 0; i < 10430; i++) {
        ASSERT_EQ(data.row(i), data2.row(i));
    }
}

TEST_F(UtilsTest, TestReadDataFromFile2) {
    // 读取数据集
    auto data = utils::read_data_from_file<float>("datasets/DSDD.txt", 58509, 48);
    ASSERT_EQ(data.rows(), 58509);
    ASSERT_EQ(data.cols(), 48);

    // 使用mmap方式读取数据集，随机对比5行验证数据是否正确
    auto data2 = utils::mmap_read_data_from_file<float>("datasets/DSDD.txt", 58509, 48);

    // 随机生成5个不同的行号
    std::vector<int> rows;
    for (int i = 0; i < 5; i++) {
        rows.push_back(random.randint(0, 58509));
    }

    // 验证数据是否正确
    for (int i = 0; i < 58509; i++) {
        ASSERT_EQ(data.row(i), data2.row(i));
    }
}

TEST_F(UtilsTest, TestReadDataFromFile3) {
    // 读取数据集
    auto start = std::chrono::steady_clock::now();
    auto data = utils::read_data_from_file<float>("datasets/KDD.txt", 4898431, 34);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "read_data_from_file: " << duration.count() << "ms" << std::endl;
    ASSERT_EQ(data.rows(), 4898431);
    ASSERT_EQ(data.cols(), 34);

    // 使用mmap方式读取数据集，随机对比5行验证数据是否正确
    start = std::chrono::steady_clock::now();
    auto data2 = utils::mmap_read_data_from_file<float>("datasets/KDD.txt", 4898431, 34);
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "mmap_read_data_from_file: " << duration.count() << "ms" << std::endl;

    // 验证数据是否正确
    for (int i = 0; i < 4898431; i++) {
        ASSERT_EQ(data.row(i), data2.row(i));
    }
}

TEST_F(UtilsTest, TestReadDataFromFile4) {
    // 读取数据集
    auto data = utils::read_data_from_file<float>("datasets/KITSUNE10.txt", 1868224, 115);
    ASSERT_EQ(data.rows(), 1868224);
    ASSERT_EQ(data.cols(), 115);
}

TEST_F(UtilsTest, TestEuclideanDistance1) {
    // 计算欧氏距离，case1: 相同点的距离为0
    for (int i = 0; i < data.rows(); i++) {
        ASSERT_EQ(utils::euclidean_distance<float>(data.row(i), data.row(i)), 0);
    }
}

TEST_F(UtilsTest, TestEuclideanDistance2) {
    // 计算欧氏距离，case2: (0, 0)与(1, 1)的距离为根号2
    Eigen::RowVectorXd point1(2);
    point1 << 0, 0;
    Eigen::RowVectorXd point2(2);
    point2 << 1, 1;
    ASSERT_EQ(utils::euclidean_distance<double>(point1, point2), std::sqrt(2));
}

TEST_F(UtilsTest, TestEuclideanDistance3) {
    // 计算欧氏距离，case3: (0, 0, 0)与(1, 1, 1)的距离为根号3
    Eigen::RowVectorXd point1(3);
    point1 << 0, 0, 0;
    Eigen::RowVectorXd point2(3);
    point2 << 1, 1, 1;
    ASSERT_EQ(utils::euclidean_distance<double>(point1, point2), std::sqrt(3));
}

