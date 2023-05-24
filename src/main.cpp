#include <iostream>
#include <thread>
#include <chrono>
#include "indicators.hpp"
#include "inifile.hpp"
#include "argparse.hpp"
#include "utils.hpp"
#include "spdlog_common.h"
#include "kmeans.hpp"
#include "log_means.hpp"
#include <eigen3/Eigen/Dense>

using namespace indicators;

// 需要加载的数据集，对应ini文件中的section
std::vector<std::string> datasets = {
    "Avila", "DSDD", "KDD", "MNIST", "KITSUNE10"
};

struct MyArgs: public argparse::Args {
    bool &all = flag("a,all", "Run all datasets");
    std::string &dataset = kwarg("d,dataset", "Dataset name [Avila/DSDD/KDD/KITSUNE10/KITSUNE/MNIST] ").set_default("Avila");
};

int main(int argc, const char *argv[]) {
    Eigen::setNbThreads(4);
    Eigen::initParallel(); 
    MyArgs args = argparse::parse<MyArgs>(argc, argv);
    util::inifile ini("config.ini");
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
            
            // 读取数据集
            auto data = utils::read_data_from_file<float>(location, num, dim);

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

        auto job = [&bar, &dataset, &location, &num, &dim, &classes]() {
            // 计算花费时间
            auto start = std::chrono::steady_clock::now();
            // 读取数据集
            auto data = utils::read_data_from_file<float>(location, num, dim);
            
            // 预估聚类数
            LogMeans log_means;
            auto k = log_means.run(data, 2, 2 * classes);
            SPDLOG_INFO("Estimated number of clusters: {}", k);

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