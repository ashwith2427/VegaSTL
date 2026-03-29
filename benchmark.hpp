#include <ArrayList.hpp>
#include <functional>

struct Benchmark {
    std::function<void()> f;
    const char *name;
};

inline ArrayList<Benchmark> &get_benchmarks()
{
    static ArrayList<Benchmark> benchmarks;
    return benchmarks;
}

struct BenchmarkRegister {
    static void register_function(const char *name, std::function<void()> f)
    {
        get_benchmarks().push_back({f, name});
    }
};

#define CONCAT(a, b) a##b
#define UNIQUE_NAME(a, b) CONCAT(a, b)

#define BENCHMARK(name)                                   \
    static void UNIQUE_NAME(benchmark_, __LINE__)();      \
    struct UNIQUE_NAME(BenchmarkReg_, __LINE__) {         \
        UNIQUE_NAME(BenchmarkReg_, __LINE__)()            \
        {                                                 \
            BenchmarkRegister::register_function(         \
                name, UNIQUE_NAME(benchmark_, __LINE__)); \
        }                                                 \
    };                                                    \
    static UNIQUE_NAME(BenchmarkReg_, __LINE__)           \
        UNIQUE_NAME(instance_, __LINE__);                 \
    static void UNIQUE_NAME(benchmark_, __LINE__)()