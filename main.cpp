#include "benchmark.hpp"
#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector.h>
void run_benchmark()
{
    constexpr int WARMUP = 2;
    constexpr int RUNS   = 5;

    std::cout << "================ BENCHMARKS ================\n\n";
    for (auto const& bench : get_benchmarks())
    {
        std::cout << "[Running] " << bench.name << "...\n";
        for (int i = 0; i < WARMUP; i++)
        {
            bench.f();
        }
        long long min_time   = std::numeric_limits<long long>::max();
        long long max_time   = 0;
        long long total_time = 0;
        for (int i = 0; i < RUNS; i++)
        {

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

// void run_tests()
// {
//     if (get_tests().size() == 0)
//     {
//         printf("No tests found\n");
//         return;
//     }
//     auto& tests = get_tests();

//     int passed = 0;

//     for (size_t i = 0; i < tests.size(); i++)
//     {
//         auto& t = tests[i];

//         try
//         {
//             t.f();
//             t.passed = true;
//             passed++;
//         }
//         catch (std::exception const& e)
//         {
//             t.passed  = false;
//             t.message = e.what();
//         }
//         catch (...)
//         {
//             t.passed  = false;
//             t.message = "Unknown error";
//         }

//         // Print result
//         if (t.passed)
//             std::cout << "[PASS] " << t.name << "\n";
//         else
//             std::cout << "[FAIL] " << t.name << " -> " << t.message << "\n";
//     }

//     std::cout << "\n====================\n";
//     std::cout << "Passed: " << passed << "/" << tests.size() << "\n";
// }

// struct Foo
// {
//     std::string name;
// };

// std::ostream& operator<<(std::ostream& os, Foo const& f)
// {
//     os << f.name;
//     return os;
// }

template <class T>
void print_T()
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

struct Point
{
    int x;
    int y;
};

#include <cstring>
#include <iostream>

struct Person
{
    char* name;
    int   age;

    // Constructor
    Person(char const* n, int a)
        : age(a)
    {
        name = new char[std::strlen(n) + 1];
        std::strcpy(name, n);
    }

    // Copy constructor (deep copy)
    Person(Person const& other)
        : age(other.age)
    {
        name = new char[std::strlen(other.name) + 1];
        std::strcpy(name, other.name);
    }

    // Move constructor
    Person(Person&& other) noexcept
        : name(other.name)
        , age(other.age)
    {
        other.name = nullptr;
    }

    // Copy assignment
    Person& operator=(Person const& other)
    {
        if (this != &other)
        {
            delete[] name;
            age  = other.age;
            name = new char[std::strlen(other.name) + 1];
            std::strcpy(name, other.name);
        }
        return *this;
    }

    // Move assignment
    Person& operator=(Person&& other) noexcept
    {
        if (this != &other)
        {
            delete[] name;
            name       = other.name;
            age        = other.age;
            other.name = nullptr;
        }
        return *this;
    }

    // Destructor
    ~Person()
    {
        delete[] name;
    }
};

std::ostream& operator<<(std::ostream& os, Person const& p)
{
    if (p.name)
        os << "Person{name: " << p.name << ", age: " << p.age << "}";
    else
        os << "Person{moved-from}";
    return os;
}
std::ostream& operator<<(std::ostream& os, Point const& p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

struct Foo
{
    Foo(char const* name)
        : name(std::move(name))
    {
    }
    char const* name;
    ~Foo() {}
};

std::ostream& operator<<(std::ostream& os, Foo const& foo)
{
    os << foo.name;
    return os;
}

int main()
{
    // vector<Foo, std::allocator<Foo>> v;
    // for (int i = 1; i < 9; i++)
    // {
    //     v.push_back(Foo{"Ashwith"});
    // }
    // v.erase(v.begin(), v.end());
    // v.insert(v.begin(), Foo{"Ashwith"});
    // v.resize(10, Foo{""});
    // for (auto it = v.begin(); it != v.end(); it++)
    // {
    //     std::cout << *it << ' ';
    // }
    // std::cout << '\n';
    run_benchmark();
    return 0;
}
