import numpy as np
from sklearn.cluster import KMeans

# 读取数据集
dataset_path = "../datasets/MNIST.txt"
data = np.loadtxt(dataset_path, delimiter=",")

# 数据集信息
num_data_points, num_dimensions = data.shape
num_clusters_range = range(2, 25)  # k 取值范围为 2 到 24

# 使用 K-Means II 聚类并计算 SSE
sse_results = []
for k in num_clusters_range:
    kmeans = KMeans(n_clusters=k, init='k-means++', n_init=1, max_iter=20, random_state=42)
    kmeans.fit(data)
    sse = kmeans.inertia_
    sse_results.append(sse)

# 输出 SSE 结果
for k, sse in zip(num_clusters_range, sse_results):
    print(f"K = {k}, SSE = {sse}")
