#ifndef __ELBOW_HPP__
#define __ELBOW_HPP__
#include "kmeans.hpp"
#include "utils.hpp"
#include "json11.hpp"

class Elbow {
public:
    Elbow(std::string dataset, std::string initMode) : initMode(initMode) {
        result = json11::Json::object({
            {"dataset", dataset},
            {"k", 0},
            {"sse", 0},
            {"sse_hist", json11::Json::array()}
        });
    }
    /**
     * @brief 预估数据集簇的数量
     * @param data 数据集
     * @param klow 最小簇数量
     * @param khigh 最大簇数量
    */
    template<typename Scalar>
    void run(Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> &data, int k_low, int k_high) {
        Scalar ssePrev1;
        Scalar ssePrev2;
        Scalar diff;
        for (int k = k_low; k <= k_high; k++) {
            KMeans kmeans(k, initMode);
            auto centroids = kmeans.fit<Scalar>(data);
            auto sse = utils::compute_sse<Scalar>(data, centroids);
            // 向sse_hist中插入一条记录(k, sse)
            auto result_items = result.object_items();
            std::vector<json11::Json> new_array = result_items["sse_hist"].array_items();
            new_array.push_back(json11::Json::array({k, sse}));
            result_items["sse_hist"] = new_array;
            result = json11::Json(result_items);

            if (k == k_low) {
                ssePrev1 = sse;
            } else if (k == k_low + 1) {
                ssePrev2 = sse;
            } else {
                diff = (ssePrev1 - ssePrev2) - (ssePrev2 - sse);
                ssePrev1 = ssePrev2;
                ssePrev2 = sse;
                std::cout << "ELBOW DIFF (K = "<< k - 1 <<") = " << diff << std::endl;
            }
            std::cout << "K =  " << k << ", SSE = " << sse << std::endl;
        }

        // 将result写入result.json文件
        std::ofstream ofs("result_elbow_" + result["dataset"].string_value() + ".json");
        ofs << result.dump();
        ofs.close();
    }
private:
    json11::Json result;
    std::string initMode;
};

#endif // __ELBOW_HPP__