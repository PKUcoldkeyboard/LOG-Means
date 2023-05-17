#include <iostream>
#include "kmeans.hpp"

int main(int argc, const char *argv[]) {
      // kmeans测试数据集
      std::vector<Eigen::VectorXd> data;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 1, 1;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 1, 2;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 2, 1;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 2, 2;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 5, 5;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 5, 6;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 6, 5;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 6, 6;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 9, 9;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 9, 10;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 10, 9;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 10, 10;
      data.push_back(Eigen::VectorXd(2));
      data.back() << 13, 13;

      // 聚类
      auto clusters = cluster::kmeans(data, 3);

      // 打印结果
      for (int i = 0; i < clusters.size(); ++i) {
          std::cout << "Cluster " << i << ": " << std::endl;
          for (auto cluster : clusters[i]) {
            std::cout << cluster << std::endl;
          }
          std::cout << std::endl;
      }

      return 0;
}