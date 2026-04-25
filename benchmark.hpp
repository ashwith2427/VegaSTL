#include <functional>
#include <vector.h>
struct Benchmark
{
    std::function<void()> f;
    char const*           name;
};

inline vector<Benchmark>& get_benchmarks()
{
    static vector<Benchmark> benchmarks;
    return benchmarks;
}

struct BenchmarkRegister
{
    static void register_function(char const* name, std::function<void()> f)
    {
        get_benchmarks().push_back(Benchmark{f, name});
    }
};

#define CONCAT(a, b) a##b
#define UNIQUE_NAME(a, b) CONCAT(a, b)

#define BENCHMARK(name)                                                                            \
    static void UNIQUE_NAME(benchmark_, __LINE__)();                                               \
    struct UNIQUE_NAME(BenchmarkReg_, __LINE__)                                                    \
    {                                                                                              \
        UNIQUE_NAME(BenchmarkReg_, __LINE__)()                                                     \
        {                                                                                          \
            BenchmarkRegister::register_function(name, UNIQUE_NAME(benchmark_, __LINE__));         \
        }                                                                                          \
    };                                                                                             \
    static UNIQUE_NAME(BenchmarkReg_, __LINE__) UNIQUE_NAME(instance_, __LINE__);                  \
    static void UNIQUE_NAME(benchmark_, __LINE__)()
