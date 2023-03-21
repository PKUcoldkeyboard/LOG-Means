<h1 align="center">LOG-Means</h1>

<div align="center"><img src="https://img.shields.io/badge/version-0.1-orange"/></div>

本项目是根据论文[1] Manuel Fritz, Michael Behringer, Holger Schwarz. LOG-Means: Efficiently Estimating the Number of Clusters in Large Datasets. PVLDB, 13(11): 2118-2131, 2020. DOI: https://doi.org/10.14778/3407790.3407813, 基于C++复现的LOG-Means算法项目。

LOG-Means算法是一种新型、简化的、高效、对大数据集和大搜索空间具有强鲁棒性的簇数目估计方法，它采用了半自动化的方式高效估计数据中的簇的个数。它具体包括三个步骤：首先对数据进行预处理，然后计算每个聚类中心的Gain值，最后确定簇的个数并进行聚类分析。实验结果表明，LOG-Means相较于13种传统的簇数估计方法，在大数据集和大搜索空间下的表现更加出色，能够更快地获得准确的估计结果。

## 算法实现

1. 预处理数据：首先，选择一个较小的样本子集，计算特征矩阵的协方差矩阵，然后用PCA（主成分分析）方法对数据集进行降维。接着，通过对降维后的数据进行K-means聚类，并计算每个数据点从其最近簇中心到数据集质心的距离，来筛选出可能的簇个数。
2. 计算聚类中心的Gain值：首先通过取不同的初始聚类中心，得到多个聚类结果，然后针对每个聚类结果，计算其聚类中心的Gain 值，包括Sum of Squares（SS）和Degrees of Freedom（Df）两个参数，用于评估每个聚类的质量和复杂性。
3. 确定簇的个数并进行聚类分析：根据前面步骤中得到的每个聚类的Gain值，采用BIC（贝叶斯信息准则）方法确定最可能的簇个数。最后使用该簇个数对整个数据集进行K-means聚类，并用Silhouette系数来评估该聚类结果的质量。

<div align="center">
  <img src="https://cuterwrite-1302252842.file.myqcloud.com/img/20230321152315.png">
</div>

## 如何使用

## 项目协作者

## 许可证
This project is licensed under the Apache License. See the LICENSE file for more information.

## 参考文献
[1] Manuel Fritz, Michael Behringer, Holger Schwarz. LOG-Means: Efficiently Estimating the Number of Clusters in Large Datasets. PVLDB, 13(11): 2118-2131, 2020. DOI: https://doi.org/10.14778/3407790.3407813
