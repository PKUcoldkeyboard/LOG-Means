<h1 align="center">LOG-Means</h1>

<div align="center">
  <img src="https://img.shields.io/badge/version-0.1-orange"/>
  <img src="https://img.shields.io/badge/Eigen3-3.3.39-blueviolet">
  <img src="https://img.shields.io/badge/Linux-passing-brightgreen">
  <img src="https://img.shields.io/badge/c%2B%2B-%3E%3D11-brightgreen">
  <img src="https://img.shields.io/github/stars/PKUcoldkeyboard/LOG-Means?style=social">
</div>

本项目是根据论文[1] Manuel Fritz, Michael Behringer, Holger Schwarz. LOG-Means: Efficiently Estimating the Number of Clusters in Large Datasets. PVLDB, 13(11): 2118-2131, 2020. DOI: https://doi.org/10.14778/3407790.3407813, 基于C++复现的LOG-Means算法项目。

LOG-Means算法是一种新型、简化的、高效、对大数据集和大搜索空间具有强鲁棒性的簇数目估计方法，它采用了半自动化的方式高效估计数据中的簇的个数。它具体包括三个步骤：首先对数据进行预处理，然后计算每个聚类中心的Gain值，最后确定簇的个数并进行聚类分析。实验结果表明，LOG-Means相较于13种传统的簇数估计方法，在大数据集和大搜索空间下的表现更加出色，能够更快地获得准确的估计结果。

## 算法实现

1. 预处理数据：首先，选择一个较小的样本子集，计算特征矩阵的协方差矩阵，然后用PCA（主成分分析）方法对数据集进行降维。接着，通过对降维后的数据进行K-means聚类，并计算每个数据点从其最近簇中心到数据集质心的距离，来筛选出可能的簇个数。
2. 计算聚类中心的Gain值：首先通过取不同的初始聚类中心，得到多个聚类结果，然后针对每个聚类结果，计算其聚类中心的Gain 值，包括Sum of Squares（SS）和Degrees of Freedom（Df）两个参数，用于评估每个聚类的质量和复杂性。
3. 确定簇的个数并进行聚类分析：根据前面步骤中得到的每个聚类的Gain值，采用BIC（贝叶斯信息准则）方法确定最可能的簇个数。最后使用该簇个数对整个数据集进行K-means聚类，并用Silhouette系数来评估该聚类结果的质量。

<div align="center">
  <img src="image/algorithm.png">
</div>

## 环境需求
- C++编译器（支持C++11或更高版本）
- 第三方库：Eigen3

## 项目模块
本项目的主要模块包括：
- 数据预处理模块（Preprocessing）
- 计算聚类中心的Gain值模块（Gain Calculation）
- 确定簇的个数并进行聚类分析模块（Cluster Analysis）
以下是这些模块之间的关系：
```
+------------------+    +---------------------+    +--------------------+
| Preprocessing    +--->+ Gain Calculation    +--->+ Cluster Analysis   |
+------------------+    +---------------------+    +--------------------+
```

## 如何使用
- 构建项目
```
./configure
```

- 运行项目
```
./build/cluster <dataset>
```

## 模块细节
### 数据预处理模块（Preprocessing）
本模块主要负责以下任务：

1. 读取输入数据。
2. 选择一个较小的样本子集。
3. 计算特征矩阵的协方差矩阵。
4. 用PCA方法对数据集进行降维。
5. 对降维后的数据进行K-means聚类。
6. 筛选出可能的簇个数。

### 计算聚类中心的Gain值模块（Gain Calculation）
本模块主要负责以下任务：

1. 通过取不同的初始聚类中心，得到多个聚类结果。
2. 针对每个聚类结果，计算其聚类中心的Gain值。
3. 计算Sum of Squares（SS）和Degrees of Freedom（Df）参数。

### 确定簇的个数并进行聚类分析模块（Cluster Analysis）
本模块主要负责以下任务：

1. 根据Gain值，采用BIC方法确定最可能的簇个数。
2. 使用该簇个数对整个数据集进行K-means聚类。
3. 用Silhouette系数评估该聚类结果的质量。

## 测试与验证
本项目将使用一系列测试用例对每个模块进行单元测试，以确保它们的正确性和有效性。测试用例应涵盖正常情况、边缘情况和异常情况。此外，我们还将使用一些具有不同特征和规模的真实数据集进行集成测试，以评估算法的性能和准确性。

## 项目协作者
<a href="https://github.com/PKUcoldkeyboard/LOG-Means/graphs/contributors">
  <img src="https://contrib.rocks./image?repo=PKUcoldkeyboard/LOG-Means" />
</a>

Made with [contrib.rocks](https://contrib.rocks.).

## 许可证
This project is licensed under the Apache 2.0 License. See the LICENSE file for more information.

## 参考文献
[1] Manuel Fritz, Michael Behringer, Holger Schwarz. LOG-Means: Efficiently Estimating the Number of Clusters in Large Datasets. PVLDB, 13(11): 2118-2131, 2020. DOI: https://doi.org/10.14778/3407790.3407813
