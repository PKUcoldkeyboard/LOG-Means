#ifndef __LOG__MEANS__HPP__
#define __LOG__MEANS__HPP__
#include "kmeans.hpp"
#include "utils.hpp"
#include "json11.hpp"
#include <queue>
#include <unordered_map>

class LogMeans {
public:
    LogMeans(std::string dataset, std::string initMode) : initMode(initMode) {
        result = json11::Json::object({
            {"dataset", dataset},
            {"k", 0},
            {"sse", 0},
            {"sse_hist", json11::Json::array()}
        });
    }

    template<typename Scalar>
    Scalar kmeans_sse(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data, int k) {
        KMeans kmeans(k, initMode);
        auto centroids = kmeans.fit<Scalar>(data);
        auto sse =  utils::compute_sse<Scalar>(data, centroids);
        // 向sse_hist中插入一条记录(k, sse)
        auto result_items = result.object_items();
        std::vector<json11::Json> new_array = result_items["sse_hist"].array_items();
        new_array.push_back(json11::Json::array({k, sse}));
        result_items["sse_hist"] = new_array;
        result = json11::Json(result_items);

        return sse;
    }
    /**
     * 预估数据集簇的数量
     * @param data 数据集
     * @param klow 最小簇数量
     * @param khigh 最大簇数量
     * @param epsilon 邻居数量
     * @return 预估的簇数量
     *
    */
    template<typename Scalar>
    int run(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data, int k_low, int k_high, Scalar epsilon = 0);

    template<typename Scalar>
    void print_sse(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data, int k_high) {
        for (int k = 1; k <= k_high; k++) {
            std::cout << "K = " << k << ", sse = " << kmeans_sse(data, k) << std::endl;
        }
    }

private:
    json11::Json result;
    std::string initMode;
    bool directly_adjacent(int k_low, int k_high) {
        return k_low + 1 == k_high;
    }
};

template<typename Scalar>
int LogMeans::run(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data, int k_low, int k_high, Scalar epsilon) {
    k_low--;
    std::unordered_map<int, Scalar> K, M;
    Scalar sseLow = kmeans_sse<Scalar>(data, k_low);
    Scalar sseHigh = kmeans_sse<Scalar>(data, k_high);
    K[k_low] = sseLow;
    K[k_high] = sseHigh;
    while (k_low < k_high && !directly_adjacent(k_low, k_high)) {
        int k_mid = (k_low + k_high) / 2;
        Scalar sseMid = kmeans_sse(data, k_mid);
        K[k_mid] = sseMid;
        Scalar radioLeft = sseLow / sseMid;
        Scalar radioRight = sseMid / sseHigh;
        M[k_mid] = radioLeft;
        M[k_high] = radioRight;

        Scalar maxRadio = 0;
        for (const auto &[k, v]: M) {
            if (v > maxRadio) {
                maxRadio = v;
                k_high = k;
            }
        }
        for (int i = k_high - 1; i >= 0; i--) {
            if (K.count(i)) {
                k_low = i;
                break;
            }
        }
        sseHigh = K[k_high];
        sseLow = K[k_low];
        std::cout << "radioLeft = " << radioLeft << ", radioRight = " << radioRight << std::endl;
        std::cout << "update: >>> k_low = " << k_low << ", k_high = " << k_high << std::endl;
    }
    // 更新result记录
    auto result_items = result.object_items();
    result_items["k"] = k_high;
    result_items["sse"] = K[k_high];
    result = json11::Json(result_items);

    // 将result写入result.json文件
    std::ofstream ofs("result_logmeans_" + result["dataset"].string_value() + ".json");
    ofs << result.dump();
    ofs.close();

    return k_high;
}

#endif // __LOG__MEANS__HPP__