## Eigen3速查表

### Eigen3 矩阵类型

| 类型  | 描述  |
| --- | --- |
| `Eigen::Matrix<T, Rows, Cols>` | 一个大小为 Rows x Cols 的矩阵，其中 T 是元素类型 |
| `Eigen::Matrix<T, Dynamic, Dynamic>` | 动态大小的矩阵，其中 T 是元素类型 |
| `Eigen::Matrix<T, Dynamic, 1>` | 动态大小的列向量，其中 T 是元素类型 |
| `Eigen::Matrix<T, 1, Dynamic>` | 动态大小的行向量，其中 T 是元素类型 |
| `Eigen::Vector3d` | 大小为 3 的 double 类型列向量 |
| `Eigen::Vector4d` | 大小为 4 的 double 类型列向量 |
| `Eigen::VectorXd` | 大小可变的 double 类型列向量 |
| `Eigen::MatrixXd` | 大小可变的 double 类型矩阵 |

### Eigen3 常用操作

| 操作  | 描述  |
| --- | --- |
| `matrix.transpose()` | 返回矩阵的转置 |
| `matrix.inverse()` | 返回矩阵的逆 |
| `matrix.diagonal()` | 返回矩阵的对角线元素 |
| `matrix.trace()` | 返回矩阵的迹 |
| `matrix.norm()` | 返回矩阵的 Frobenius 范数 |
| `matrix.col(index)` | 返回矩阵的第 index 列 |
| `matrix.row(index)` | 返回矩阵的第 index 行 |
| `matrix.block(i, j, rows, cols)` | 返回以 (i, j) 为起始位置，大小为 rows x cols 的子矩阵 |
| `matrix.topRows(n)` | 返回矩阵的前 n 行 |
| `matrix.bottomRows(n)` | 返回矩阵的后 n 行 |
| `matrix.leftCols(n)` | 返回矩阵的左边 n 列 |
| `matrix.rightCols(n)` | 返回矩阵的右边 n 列 |

### Eigen3 常用运算符

| 运算符 | 描述  |
| --- | --- |
| `*` | 矩阵乘法 |
| `+` | 矩阵加法 |
| `-` | 矩阵减法 |
| `*=` | 矩阵乘法赋值 |
| `+=` | 矩阵加法赋值 |
| `-=` | 矩阵减法赋值 |
| `==` | 判断两个矩阵是否相等 |
| `!=` | 判断两个矩阵是否不相等 |

### Eigen3 随机数生成

Eigen3 库中的 `Eigen::MatrixXd::Random(rows, cols)` 函数可以生成一个大小为 rows x cols 的随机矩阵，元素取值范围为 [-1, 1]。

### Eigen3 线性代数函数

Eigen3 库提供了很多常用的线性代数函数，如求解线性方程组、求解特征值和特征向量、SVD 分解等。以下是一些常用的函数：

| 函数  | 描述  |
| --- | --- |
| `Eigen::FullPivLU` | 求解矩阵的全选主元 LU 分解 |
| `Eigen::PartialPivLU` | 求解矩阵的部分选主元 LU 分解 |
| `Eigen::HouseholderQR` | 求解矩阵的 Householder QR 分解 |
| `Eigen::ColPivHouseholderQR` | 求解矩阵的列主元 Householder QR 分解 |
| `Eigen::FullPivHouseholderQR` | 求解矩阵的全选主元 Householder QR 分解 |
| `Eigen::JacobiSVD` | 求解矩阵的奇异值分解 |
| `Eigen::SelfAdjointEigenSolver` | 求解对称矩阵的特征值和特征向量 |
| `Eigen::GeneralizedSelfAdjointEigenSolver` | 求解对称广义矩阵的特征值和特征向量 |
| `Eigen::EigenSolver` | 求解一般矩阵的特征值和特征向量 |

这些函数的用法可以参考 Eigen3 官方文档。

### Eigen3 运行时性能优化

Eigen3 库支持一些运行时性能优化选项，例如启用多线程和使用 SIMD 指令。可以通过以下方式启用这些选项：

```cpp
Eigen::setNbThreads(4); // 设置线程数为 4
Eigen::initParallel();  // 启用多线程
Eigen::setNbThreads(1); // 关闭多线程

Eigen::VectorXd a, b, c;
c = a.array() * b.array(); // 启用 SIMD 指令

```

其中 `setNbThreads` 函数用于设置线程数，`initParallel` 函数用于启用多线程，`array` 函数用于将矩阵转换为数组，从而启用 SIMD 指令。

### 矩阵与矩阵乘法、点乘

```cpp
#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;

int main()
{
    Matrix2d A;
    A << 1, 2, 3, 4;
    Matrix2d B;
    B << 5, 6, 7, 8;

    // 矩阵乘法
    Matrix2d C = A * B;
    std::cout << "A * B =\n" << C << std::endl;

    // 矩阵点乘
    Matrix2d D = A.array() * B.array();
    std::cout << "A . B =\n" << D << std::endl;

    return 0;
}

```

### 矩阵与向量乘法、点乘

```cpp
#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;

int main()
{
    Matrix2d A;
    A << 1, 2, 3, 4;
    Vector2d v(5, 6);

    // 矩阵与向量乘法
    Vector2d u = A * v;
    std::cout << "A * v =\n" << u << std::endl;

    // 矩阵点乘向量
    Vector2d w = A.array() * v.array();
    std::cout << "A . v =\n" << w << std::endl;

    return 0;
}

```

### 向量与向量乘法、点乘

```cpp
#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;

int main()
{
    Vector3d v(1, 2, 3);
    Vector3d w(4, 5, 6);

    // 向量乘法
    Vector3d u = v.cross(w);
    std::cout << "v x w =\n" << u << std::endl;

    // 向量点乘
    double s = v.dot(w);
    std::cout << "v . w = " << s << std::endl;

    return 0;
}
```