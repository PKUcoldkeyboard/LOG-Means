#include <iostream>
#include <eigen3/Eigen/Dense>

int main(int argc, const char* argv[]) {
    Eigen::Matrix3d m1;
    m1 << 1, 2, 3,
          4, 5, 6,
          7, 8, 9;
    Eigen::Matrix3d m2;
    m2 << 10, 11, 12,
          13, 14, 15,
          16, 17, 18;
    Eigen::Vector3d v1(1, 2, 3);
    Eigen::Vector3d v2(4, 5, 6);
    
    Eigen::Matrix3d result1 = m1 * m2; // 矩阵乘法
    Eigen::Vector3d result2 = v1 + v2; // 向量加法
    
    std::cout << "Matrix1 * Matrix2:\n" << result1 << std::endl;
    std::cout << "Vector1 + Vector2:\n" << result2 << std::endl;
    
    return 0;
}