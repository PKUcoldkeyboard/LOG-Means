#include <iostream>
#include "kmeans.hpp"

int main(int argc, const char *argv[]) {
      // kmeans测试数据集
    std::vector<Eigen::VectorXd> data;
    data.push_back(Eigen::VectorXd(2));
    data.back() << 1.0, 1.0;
    data.push_back(Eigen::VectorXd(2));
    data.back() << 1.5, 2.0;
    data.push_back(Eigen::VectorXd(2));
    data.back() << 3.0, 4.0;
    data.push_back(Eigen::VectorXd(2));
    data.back() << 5.0, 7.0;
    data.push_back(Eigen::VectorXd(2));
    data.back() << 3.5, 5.0;
    data.push_back(Eigen::VectorXd(2));
    data.back() << 4.5, 5.0;
    data.push_back(Eigen::VectorXd(2));
    data.back() << 3.5, 4.5;

    // 聚类
    KMeans kmeans(2);
    auto result = kmeans.fit(data);

    // 打印结果
    std::cout << "聚类结果：" << std::endl;
    for (int i = 0; i < result.first.size(); ++i) {
        std::cout << "簇" << i << "：" << std::endl;
        for (auto &point : result.first[i].second) {
            std::cout << point.transpose() << std::endl;
        }
    }
    std::cout << "标签：" << std::endl;
    for (auto &label : result.second) {
        std::cout << label << " ";
    }
    std::cout << std::endl;
    
    return 0;
}