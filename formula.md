# 数学公式速查表

## 1. SSE (Sum of Squared Errors)

SSE 是评估聚类质量的一个参数，计算公式如下：

$$
SSE = \sum\limits_{k=1}^K \sum\limits_{x_i \in C_k} \Vert x_i - \mu_k \Vert^2
$$

其中，$K$表示聚类数量，$C_k$表示第k个簇，$x_i$是属于簇k的数据点，$\mu_k$是簇k的中心。

## 2. SSE比率
SSE比率是用于评估簇数量变化对聚类效果影响的一个参数，计算公式如下：

$$
SSE_{ratio} = \frac{SSE_{k-1}}{SSE_k}
$$

其中，$SSE(K)$表示K个簇时的SSE值，$SSE(k - 1)$表示k - 1个簇时的SSE值。

## 3. K-means聚类
在K-means聚类中，聚类中心更新公式为：

$$
\mu_{k}=\frac{1}{\left|C_{k}\right|} \sum_{x_{i} \in C_{k}} x_{i}
$$

其中，$\mu_k$是簇k的中心，$|C_k|$表示簇k的样本数，$x_i$是属于簇k的数据点。


## 4. Degrees of Freedom(DF)

Degrees of Freedom (Df) 用于评估聚类的复杂性，计算公式如下：

$$
DF = K \times (d+1)
$$

其中，$K$表示聚类数量，$d$表示数据的特征数。

## 5. BIC(贝叶斯信息准则)

BIC用于选择最佳模型，计算公式如下：

$$
BIC = -2 \ln L + DF \times \ln n
$$

其中，$L$表示模型的似然函数，$Df$表示自由度，$n$表示样本数。

## 6. Silhouette系数

Silhouette系数用于评估聚类结果的质量，计算公式如下：

$$
s(i) = \frac{b(i) - a(i)}{\max (a(i), b(i))}
$$