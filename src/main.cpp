#include <iostream>
#include <thread>
#include <chrono>
#include "indicators.hpp"
#include "inifile.hpp"
#include "argparse.hpp"
#include "utils.hpp"
#include "spdlog_common.h"
#include <eigen3/Eigen/Dense>

using namespace indicators;

// 需要加载的数据集，对应ini文件中的section
std::vector<std::string> datasets = {
    "Avila", "DSDD", "KDD", "MNIST", "KITSUNE"
};

struct MyArgs: public argparse::Args {
    bool &all = flag("a,all", "Run all datasets");
    std::string &dataset = kwarg("d,dataset", "Dataset name [Avila/DSDD/KDD/KITSUNE/MNIST] ").set_default("Avila");
};

int main(int argc, const char *argv[]) {
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

            // 预估聚类数
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 + cnt * 200));

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

        Eigen::MatrixXf data = utils::read_data_from_file<float>(location, num, dim);
        std::cout << data.block(0, 0, 10, dim) << std::endl;
        // auto job = [&bar, &dataset, &location, &num, &dim, &classes]() {
        //     // 读取数据集
        //     Eigen::MatrixXd data = utils::read_data_from_file(location, num, dim);

        //     // 打印data前十行, std
        //     std::cout << data.block(0, 0, 10, dim) << std::endl;

        //     bar.mark_as_completed();
        //     SPDLOG_INFO("Done Running dataset {}", dataset);
        // };
        // std::thread job_completion_thread(job);

        // while (!bar.is_completed()) {
        //     bar.tick();
        //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // }
        // job_completion_thread.join();
        // show_console_cursor(true);
    }
    return 0;
}