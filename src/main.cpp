#include <iostream>
#include <thread>
#include <chrono>
#include <omp.h>
#include "indicators.hpp"
#include "inifile.hpp"
#include "argparse.hpp"
#include "utils.hpp"
#include "spdlog_common.h"
#include "kmeans.hpp"
#include "elbow.hpp"
#include "log_means.hpp"
#include <eigen-3.4.0/Eigen/Dense>
#ifndef EIGEN_USE_MKL_ALL
#define EIGEN_USE_MKL_ALL
#endif

#ifndef EIGEN_VECTORIZE_SSE4_2
#define EIGEN_VECTORIZE_SSE4_2
#endif

using namespace indicators;

// 需要加载的数据集，对应ini文件中的section
std::vector<std::string> datasets = {
    "Avila", "DSDD", "KDD", "MNIST", "KITSUNE10"
};

struct MyArgs: public argparse::Args {
    bool &elbow = flag("e,elbow", "Run elbow method");
    bool &all = flag("a,all", "Run all datasets");
    // 大范围搜索模式
    bool &search = flag("s,search", "Large search space mode - k: [2, 2c] or k: [2, 10c]");
    std::string &dataset = kwarg("d,dataset", "Dataset name [Avila/DSDD/KDD/KITSUNE10/KITSUNE/MNIST] ").set_default("Avila");
    std::string &initMode = kwarg("i,init", "Init mode [random/kmeans++/kmeans||)]").set_default("kmeans||");
};

int main(int argc, const char *argv[]) {
    // 设置OpenMP线程数
    auto nthreads = std::thread::hardware_concurrency();
    omp_set_num_threads(nthreads);
    Eigen::setNbThreads(nthreads);
    Eigen::initParallel(); 
    MyArgs args = argparse::parse<MyArgs>(argc, argv);
    util::inifile ini("config.ini");
    auto initMode = args.initMode;
    if (args.all) {
        ProgressBar bar{option::BarWidth{50},
            option::Start{"["},
            option::Fill{"="},
            option::Lead{">"},
            option::Remainder{" "},
            option::End{"]"},
            option::ForegroundColor{Color::white},
            option::ShowElapsedTime{true},
            option::ShowRemainingTime{true},
            option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};
        SPDLOG_INFO("Running all datasets");
        int cnt = 0;
        for (const auto &dataset : datasets) {
            std::string prefix = "Running dataset " + dataset + " " + std::to_string(cnt) + "/5 ";
            bar.set_option(option::PrefixText{prefix});
            bar.set_progress(cnt * 19);

            auto location = ini.section(dataset).get_string("location");
            auto num = ini.section(dataset).get_int("num");
            auto dim = ini.section(dataset).get_int("dim");
            auto classes = ini.section(dataset).get_int("classes");

            auto start = std::chrono::steady_clock::now();
            // 读取数据集
            auto data = utils::ifs_read_data_from_file<float>(location, num, dim);

            // 预估聚类数
            int k = 0;
            if (args.elbow) {
                Elbow elbow(dataset, initMode);
                // 搜索范围为 [2, 2c]
                int k_low = 2;
                int k_high = args.search ? 10 * classes : 2 * classes;
                // 半自动， 不需要输出
                elbow.run<float>(data, k_low, k_high);
                SPDLOG_INFO("You should choose the number of clusters from the elbow plot.");
            } else {
                LogMeans log_means(dataset, initMode);
                // 搜索范围为 [0.5c, 2c]
                int k_low = classes / 2;
                int k_high = args.search ? 10 * classes : 2 * classes;
                k = log_means.run<float>(data, k_low, k_high);
                SPDLOG_INFO("Estimated number of clusters: {}", k);
                float delta = utils::compute_delta(k, classes);
                SPDLOG_INFO("Delta: {:.5f}", delta);
            }

            SPDLOG_INFO("Done Running dataset {}", dataset);
            
            auto end = std::chrono::steady_clock::now();
            SPDLOG_INFO("Time elapsed: {}ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

            cnt++;
            if (cnt == 5) {
                bar.set_option(option::PrefixText{"Done running 5/5 "});
                bar.set_progress(100);
            }
        }

    } else {
        IndeterminateProgressBar bar{
            option::BarWidth{50},
            option::Start{"["},
            option::Fill{"."},
            option::Lead{"<==>"},
            option::End{"]"},
            option::PostfixText{"Running dataset " + args.dataset},
            option::ForegroundColor{Color::white},
            option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
        };
        show_console_cursor(false);

        SPDLOG_INFO("Running dataset {}", args.dataset);
        const auto& dataset = args.dataset;
        auto location = ini.section(dataset).get_string("location");
        auto num = ini.section(dataset).get_int("num");
        auto dim = ini.section(dataset).get_int("dim");
        auto classes = ini.section(dataset).get_int("classes");

        // 打印数据集信息
        SPDLOG_INFO("Location: {}", location);
        SPDLOG_INFO("Number of samples: {}", num);
        SPDLOG_INFO("Dimension: {}", dim);
        SPDLOG_INFO("Number of classes: {}", classes);
        SPDLOG_INFO("Init Mode: {}", args.initMode);

        auto job = [&bar, &dataset, &location, &num, &dim, &classes, &args, &initMode]() {
            // 计算花费时间
            auto start = std::chrono::steady_clock::now();
            // 读取数据集
            auto data = utils::ifs_read_data_from_file<float>(location, num, dim);
            
            // 预估聚类数
            int k = 0;
            if (args.elbow) {
                Elbow elbow(dataset, initMode);
                // 搜索范围为 [2, 2c]
                int k_low = 2;
                int k_high = args.search ? 10 * classes : 2 * classes;
                // 半自动， 不需要输出
                elbow.run<float>(data, k_low, k_high);
                SPDLOG_INFO("You should choose the number of clusters from the elbow plot.");
            } else {
                LogMeans log_means(dataset, initMode);
                // 搜索范围为 [0.5c, 2c]
                int k_low = classes / 2;
                int k_high = args.search ? 10 * classes : 2 * classes;
                k = log_means.run<float>(data, k_low, k_high);
                SPDLOG_INFO("Estimated number of clusters: {}", k);
                float delta = utils::compute_delta(k, classes);
                SPDLOG_INFO("Delta: {:.5f}", delta);
            }
            bar.mark_as_completed();
            SPDLOG_INFO("Done Running dataset {}", dataset);
            auto end = std::chrono::steady_clock::now();
            SPDLOG_INFO("Time elapsed: {}ms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
        };
        std::thread job_completion_thread(job);

        while (!bar.is_completed()) {
            bar.tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        job_completion_thread.join();
        show_console_cursor(true);
    }
    return 0;
}