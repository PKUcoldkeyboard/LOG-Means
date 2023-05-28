import numpy as np
from sklearn.cluster import KMeans

# 读取数据集
dataset_path = "../datasets/Avila.txt"
data = np.loadtxt(dataset_path, delimiter=",")

# 数据集信息
num_data_points, num_dimensions = data.shape
num_clusters_range = range(1, 25)  # k 取值范围为 2 到 24

# 使用 K-Means II 聚类并计算 SSE
for k in num_clusters_range:
    kmeans = KMeans(n_clusters=k, init='random', n_init=1, max_iter=20)
    kmeans.fit(data)
    sse = kmeans.inertia_
    print(f"K = {k}, SSE = {sse}")
