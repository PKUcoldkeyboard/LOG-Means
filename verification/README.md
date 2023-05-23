# 验证
该目录包含一个用于对Avila数据集应用K均值算法并输出SSE（Sum of Squared Errors，平方误差和）结果的Python脚本。

## 文件说明
kmeans_clustering.py：这个Python脚本使用K均值算法对给定的数据集进行聚类，并计算SSE结果。

## 依赖项
请确保已安装以下依赖项：

- Python 3.x
- scikit-learn：pip install scikit-learn

## 使用方法
- 使用以下命令运行脚本：python kmeans_clustering.py。
- 脚本将对Avila数据集执行K均值聚类，并显示SSE结果。

## 结果
该脚本将输出SSE结果，这是衡量聚类质量的指标。SSE值越低，聚类性能越好。