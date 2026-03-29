#include "benchmark.hpp"
#include <chrono>
#include <iostream>
#include <limits>

BENCHMARK("push_back_10M")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e7; i++) {
        a.push_back(i);
    }
}

BENCHMARK("push_pop_mix")
{
    ArrayList<int> a;

    for (int i = 0; i < 1e6; i++) {
        a.push_back(i);
    }

    for (int i = 0; i < 5e5; i++) {
        a.pop_back();
    }

    for (int i = 0; i < 5e5; i++) {
        a.push_back(i);
    }
}


BENCHMARK("resize_test")
{
    ArrayList<int> a;
    a.resize(1e6);
    a.resize(5e5);
    a.resize(1e6);
}


BENCHMARK("reserve_reuse")
{
    ArrayList<int> a;
    a.reserve(1e7);

    for (int i = 0; i < 1e7; i++) {
        a.push_back(i);
    }

    a.resize(0);

    for (int i = 0; i < 1e7; i++) {
        a.push_back(i);
    }
}


BENCHMARK("iteration")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e7; i++) {
        a.push_back(i);
    }

    volatile long long sum = 0;
    for (size_t i = 0; i < a.size(); i++) {
        sum += a[i];
    }
}


BENCHMARK("erase_middle_small")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e5; i++) {
        a.push_back(i);
    }

    for (int i = 0; i < 1000; i++) {
        a.erase(a.size() / 2);
    }
}

BENCHMARK("erase_back")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e6; i++) {
        a.push_back(i);
    }

    for (int i = 0; i < 1e6; i++) {
        a.pop_back();
    }
}

int main()
{
    constexpr int WARMUP = 2;
    constexpr int RUNS = 5;

    std::cout << "================ BENCHMARKS ================\n\n";
    for (const auto &bench : get_benchmarks()) {
        std::cout << "[Running] " << bench.name << "...\n";
        for (int i = 0; i < WARMUP; i++) {
            bench.f();
        }
        long long min_time = std::numeric_limits<long long>::max();
        long long max_time = 0;
        long long total_time = 0;
        for (int i = 0; i < RUNS; i++) {

            auto start = std::chrono::high_resolution_clock::now();

            bench.f();

            auto end = std::chrono::high_resolution_clock::now();

            auto duration =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            min_time = std::min(min_time, duration);
            max_time = std::max(max_time, duration);
            total_time += duration;
        }

        long long avg_time = total_time / RUNS;
        std::cout << "--------------------------------------------\n";
        std::cout << "Benchmark : " << bench.name << "\n";
        std::cout << "Runs      : " << RUNS << "\n";
        std::cout << "Min       : " << min_time / 1000.0 << " ms\n";
        std::cout << "Avg       : " << avg_time / 1000.0 << " ms\n";
        std::cout << "Max       : " << max_time / 1000.0 << " ms\n";
        std::cout << "--------------------------------------------\n\n";
    }

    std::cout << "============= DONE =============\n";
    return 0;
}