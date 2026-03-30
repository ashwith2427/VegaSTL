#include <ArrayList.hpp>
#include <functional>

struct Test {
    std::function<void()> f;
    const char *name;
};

inline ArrayList<Test> &get_tests()
{
    static ArrayList<Test> tests;
    return tests;
}

struct TestRegister {
    static void register_function(const char *name, std::function<void()> f)
    {
        get_tests().push_back({f, name});
    }
};

#define CONCAT(a, b) a##b
#define UNIQUE_NAME(a, b) CONCAT(a, b)

#define TEST(name)                                   \
    static void UNIQUE_NAME(test_, __LINE__)();      \
    struct UNIQUE_NAME(TestReg_, __LINE__) {         \
        UNIQUE_NAME(TestReg_, __LINE__)()            \
        {                                            \
            TestRegister::register_function(         \
                name, UNIQUE_NAME(test_, __LINE__)); \
        }                                            \
    };                                               \
    static UNIQUE_NAME(TestReg_, __LINE__)           \
        UNIQUE_NAME(instance_, __LINE__);            \
    static void UNIQUE_NAME(test_, __LINE__)()