<h1 align="center">LOG-Means</h1>

<div align="center">
  <img src="https://img.shields.io/badge/version-0.1-orange"/>
  <img src="https://img.shields.io/badge/Eigen3-3.3.39-blueviolet">
  <img src="https://img.shields.io/badge/Linux-passing-brightgreen">
  <img src="https://img.shields.io/badge/c%2B%2B-%3E%3D11-brightgreen">
  <img src="https://img.shields.io/github/stars/PKUcoldkeyboard/LOG-Means?style=social">
</div>

本项目是根据论文[1] Manuel Fritz, Michael Behringer, Holger Schwarz. LOG-Means: Efficiently Estimating the Number of Clusters in Large Datasets. PVLDB, 13(11): 2118-2131, 2020. DOI: https://doi.org/10.14778/3407790.3407813, 基于C++复现的LOG-Means算法项目。

LOG-Means算法是一种新型、简化的、高效、对大数据集和大搜索空间具有强鲁棒性的簇数目估计方法。它采用了二分搜索策略和递归细化策略，分别在大范围和小范围内进行簇数目估计，从而高效估计数据中的簇的个数。

## 算法实现

- 初始化：设定簇数量的最小值和最大值，以及邻居数量。
- 大范围搜索：使用二分搜索策略，在簇数量的最小值和最大值之间进行搜索。对每个簇数量，使用K-means算法计算其对应的SSE，并计算相邻簇数量之间的SSE比率。将簇数量和比率的对应关系存储在映射中。
- 递归细化：如果设定了邻居数量，算法将在最大比率的簇数量的邻域内进行更精细的搜索。再次计算每个簇数量的SSE和相应的比率，并更新映射。
- 选择簇数量：选择映射中比率最大的簇数量作为最终的簇数量估计。

<div align="center">
  <img src="image/algorithm.png">
</div>

## 环境需求
- C++编译器（支持C++11或更高版本）
- 第三方库：Eigen3

## 项目模块
本项目的主要模块包括：

- 大范围搜索模块（Large-Scale Search）
- 递归细化模块（Recursive Refinement）
- 选择簇数量模块（Cluster Number Selection）
以下是这些模块之间的关系：
```
+---------------------+    +-----------------------+    +--------------------------+
| Large-Scale Search  +---> | Recursive Refinement +--->+ Cluster Number Selection |
+---------------------+    +-----------------------+    +--------------------------+
```

## 如何使用
- 构建项目
```
./configure
```

- 运行项目
```
./cluster <dataset>
```

## 模块细节
### 大范围搜索模块（Large-Scale Search）
本模块主要负责以下任务：

1. 读取输入数据。
2. 设定簇数量的最小值和最大值。
3. 使用二分搜索策略，在簇数量的最小值和最大值之间进行搜索。
4. 对每个簇数量，使用K-means算法计算其对应的SSE。
5. 计算相邻簇数量之间的SSE比率。
6. 将簇数量和比率的对应关系存储在映射中。

### 递归细化模块（Recursive Refinement）
本模块主要负责以下任务：

1. 在最大比率的簇数量的邻域内进行更精细的搜索（如果设定了邻居数量）。
2. 计算每个簇数量的SSE和相应的比率。
3. 更新映射。

### 选择簇数量模块（Cluster Number Selection）
本模块主要负责以下任务：

1. 从映射中选择比率最大的簇数量。
2. 将选定的簇数量作为最终的簇数量估计。

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
