#include "benchmark.hpp"
BENCHMARK("push_back_100M")
{
    vector<int> a;
    for (int i = 0; i < 1e8; i++)
    {
        a.push_back(i);
    }
}

BENCHMARK("push_pop_huge")
{
    vector<int> a;
    for (int i = 0; i < 1e7; i++)
        a.push_back(i);
    for (int i = 0; i < 5e6; i++)
        a.pop_back();
    for (int i = 0; i < 5e6; i++)
        a.push_back(i);
}

BENCHMARK("resize_pingpong")
{
    vector<int> a;
    for (int j = 0; j < 50; j++)
    { // repeat 50 times
        a.resize(1e6);
        a.resize(5e5);
    }
}

BENCHMARK("erase_middle_huge")
{
    vector<int> a;
    for (int i = 0; i < 1e6; i++)
        a.push_back(i);
    for (int i = 0; i < 1e5; i++)
        a.erase(a.begin() + (a.size() / 2)); // 100k middle erases
}

BENCHMARK("erase_back_huge")
{
    vector<int> a;
    for (int i = 0; i < 1e7; i++)
        a.push_back(i);
    for (int i = 0; i < 1e7; i++)
        a.pop_back();
}

BENCHMARK("erase_unordered_huge")
{
    vector<int> a;
    for (int i = 0; i < 1e7; i++)
        a.push_back(i);
    for (int i = 0; i < 1e6; i++)
        a.erase_unordered(a.begin() + (i % a.size()));
}

BENCHMARK("iteration_huge")
{
    vector<int> a;
    for (int i = 0; i < 1e7; i++)
        a.push_back(i);
    long long volatile sum = 0;
    for (size_t i = 0; i < a.size(); ++i)
        sum += a[i];
}

BENCHMARK("random_access_huge")
{
    vector<int> a;
    for (int i = 0; i < 1e7; i++)
        a.push_back(i);

    for (int i = 0; i < 1e6; i++)
    {
        size_t idx = rand() % a.size();
        if (i % 2 == 0)
            a.erase_unordered(a.begin() + idx);
        else
            a.push_back(i);
    }
}

BENCHMARK("reserve_massive")
{
    vector<int> a;
    a.reserve(2e8); // massive reserve to test large allocations
    for (int i = 0; i < 2e8; i++)
        a.push_back(i);
    a.resize(0);
    for (int i = 0; i < 2e8; i++)
        a.push_back(i);
}
