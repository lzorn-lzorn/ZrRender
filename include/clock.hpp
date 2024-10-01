#pragma once

#include "../include/utilities.hpp"

namespace ZrRender{

class clock{
public:
    clock() : start_time(), end_time(), is_running(false) {}

    void start() {
        start_time = std::chrono::high_resolution_clock::now();
        is_running = true;
    }

    void stop() {
        end_time = std::chrono::high_resolution_clock::now();
        is_running = false;
    }

    double elapsed() const {
        if (is_running) {
            auto current_time = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<double>(current_time - start_time).count();
        } else {
            return std::chrono::duration<double>(end_time - start_time).count();
        }
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool is_running;
};    
}