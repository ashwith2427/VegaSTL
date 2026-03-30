#include "ArrayList.hpp"
#include "benchmark.hpp"
#include "test.hpp"
#include <assert.h>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <limits>
#include <thread>

// ================= EXTREME STRESS BENCHMARKS =================

BENCHMARK("push_back_100M")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e8; i++) {
        a.push_back(i);
    }
}

BENCHMARK("push_pop_huge")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e7; i++)
        a.push_back(i);
    for (int i = 0; i < 5e6; i++)
        a.pop_back();
    for (int i = 0; i < 5e6; i++)
        a.push_back(i);
}

BENCHMARK("resize_pingpong")
{
    ArrayList<int> a;
    for (int j = 0; j < 50; j++) { // repeat 50 times
        a.resize(1e6);
        a.resize(5e5);
    }
}

BENCHMARK("erase_middle_huge")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e6; i++)
        a.push_back(i);
    for (int i = 0; i < 1e5; i++)
        a.erase(a.size() / 2); // 100k middle erases
}

BENCHMARK("erase_back_huge")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e7; i++)
        a.push_back(i);
    for (int i = 0; i < 1e7; i++)
        a.pop_back();
}

BENCHMARK("erase_unordered_huge")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e7; i++)
        a.push_back(i);
    for (int i = 0; i < 1e6; i++)
        a.erase_unordered(i % a.size());
}

BENCHMARK("iteration_huge")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e7; i++)
        a.push_back(i);
    volatile long long sum = 0;
    for (int j = 0; j < 100; j++) { // repeat 100 times
        for (size_t i = 0; i < a.size(); i++)
            sum += a[i];
    }
}

BENCHMARK("random_access_huge")
{
    ArrayList<int> a;
    for (int i = 0; i < 1e7; i++)
        a.push_back(i);

    for (int i = 0; i < 1e6; i++) {
        size_t idx = rand() % a.size();
        if (i % 2 == 0)
            a.erase_unordered(idx);
        else
            a.push_back(i);
    }
}

BENCHMARK("reserve_massive")
{
    ArrayList<int> a;
    a.reserve(2e8); // massive reserve to test large allocations
    for (int i = 0; i < 2e8; i++)
        a.push_back(i);
    a.resize(0);
    for (int i = 0; i < 2e8; i++)
        a.push_back(i);
}

BENCHMARK("Parallel ArrayList")
{
}

TEST("Multithreading")
{
    ArrayList<int> list;
    std::thread p1(
        [](ArrayList<int> *list) {
            list->push_back(4);
        },
        &list);
    std::thread p2(
        [](ArrayList<int> *list) {
            list->push_back(5);
        },
        &list);

    p1.join();
    p2.join();

    assert(list[0] == 4);
}


void run_benchmark()
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
}

void run_tests()
{
    if (get_tests().size() == 0) {
        printf("No tests found\n");
        return;
    }
    printf("===============================Running Tests===============================\n");
    for (const auto &test : get_tests()) {
        printf("----------------------------------------------\n");
        printf("Running %s\n", test.name);
        printf("----------------------------------------------\n");
        test.f();
        printf("----------------------------------------------\n");
    }
    printf("===================================Done====================================\n");
}

int main()
{
    run_tests();
    return 0;
}