import numpy as np
from sklearn.cluster import KMeans

# K-Means|| 算法
class ScalableKMeansPP:
    def __init__(self, data, k, l):
        self.data = data
        self.k = k 
        self.l = l 

    def KMeansParallel(data, k, l):
        N = data.__len__()
        if k <= 0 or not(isinstance(k,int)) or l <= 0:
            sys.exit()
        # Then we start to Implement the algorithm
        # 1. Sample one point uniformly at random from X
        c = np.array(data[np.random.choice(range(N),1),])
        # 2. To Cost function
        phi = ScalableKMeansPP.CostFunction(c, data)
        # 3. Looping
        for i in range(np.ceil(np.log(phi)).astype(int)):
            cPrime = data[ScalableKMeansPP.SamplingProbability(c,data,l) > np.random.uniform(size = N),]
            c = np.concatenate((c, cPrime))
        # End looping
        # 7. For x in C, set w_x to be the number of pts closest to X
        cMini = [np.argmin(np.sum((c-pts)**2,axis=1)) for pts in data];
        closerPts = [cMini.count(i) for i in range(len(c))]
        weight = closerPts/np.sum(closerPts)
        # 8. Recluster the weighted points in C into k clusters
        allC = data[np.random.choice(range(len(c)),size=1,p=weight),]
        data_final = c
        for i in range(k-1):
            Probability = ScalableKMeansPP.SamplingProbability(allC,data_final,l) * weight
            # choose next centroid
            cPrimeFin = data[np.random.choice(range(len(c)), size=1, p=Probability/np.sum(Probability)),]
            allC = np.concatenate((allC,cPrimeFin))
        KMeansPP = KMeans(n_clusters=k, n_init=1, init=allC, max_iter=20, tol=0.0001)
        KMeansPP.fit(data);
        return KMeansPP
    
    def SamplingProbability(c,data,l):
        cost = ScalableKMeansPP.CostFunction(c,data)
        return np.array([(min(np.sum((c-pts)**2,axis=1))) * l / cost for pts in data])
    
    def CostFunction(c,data):
        return np.sum([min(np.sum((c-pts)**2,axis=1)) for pts in data]) 

# 读取数据集
dataset_path = "../datasets/Avila.txt"
data = np.loadtxt(dataset_path, delimiter=",")

# 数据集信息
num_data_points, num_dimensions = data.shape
num_clusters_range = range(1, 25)  # k 取值范围为 2 到 24

# 使用 K-Means II 聚类并计算 SSE
for k in num_clusters_range:
    # kmeans = KMeans(n_clusters=k, init='k-means++', n_init=1, max_iter=20)
    # kmeans.fit(data)
    ScalableKMeansPP.KMeansParallel(data, k, 2 * k)
    KMeansPP = ScalableKMeansPP.KMeansParallel(data, k, 2 * k)
    sse = KMeansPP.inertia_
    print(f"K = {k}, SSE = {sse}")
