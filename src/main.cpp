#include <iostream>
#include <thread>
#include <chrono>
#include "indicators.hpp"

using namespace indicators;

int main(int argc, const char *argv[]) {
    ProgressBar bar{option::BarWidth{50},
                    option::Start{"["},
                    option::Fill{"="},
                    option::Lead{">"},
                    option::Remainder{" "},
                    option::End{"]"},
                    option::PrefixText{"Computing estimated number of cluster "},
                    option::ForegroundColor{Color::yellow},
                    option::ShowElapsedTime{true},
                    option::ShowRemainingTime{true},
                    option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};
    // Update bar state
    while (true) {
        bar.tick();
        if (bar.is_completed()) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}