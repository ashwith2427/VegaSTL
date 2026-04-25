// #include <functional>
// #include <vector.hpp>

// #define ASSERT(cond)                                                                               \
//     do                                                                                             \
//     {                                                                                              \
//         if (!(cond))                                                                               \
//         {                                                                                          \
//             throw std::runtime_error(#cond " failed");                                             \
//         }                                                                                          \
//     } while (0)

// struct Test
// {
//     std::function<void()> f;
//     char const*           name;

//     bool        passed  = true;
//     char const* message = "";
// };

// inline vega::vector<Test>& get_tests()
// {
//     static vega::vector<Test> tests;
//     return tests;
// }

// struct TestRegister
// {
//     static void register_function(char const* name, std::function<void()> f)
//     {
//         get_tests().push_back({f, name});
//     }
// };

// #define CONCAT(a, b) a##b
// #define UNIQUE_NAME(a, b) CONCAT(a, b)

// #define TEST(name)                                                                                 \
//     static void UNIQUE_NAME(test_, __LINE__)();                                                    \
//     struct UNIQUE_NAME(TestReg_, __LINE__)                                                         \
//     {                                                                                              \
//         UNIQUE_NAME(TestReg_, __LINE__)()                                                          \
//         {                                                                                          \
//             TestRegister::register_function(name, UNIQUE_NAME(test_, __LINE__));                   \
//         }                                                                                          \
//     };                                                                                             \
//     static UNIQUE_NAME(TestReg_, __LINE__) UNIQUE_NAME(instance_, __LINE__);                       \
//     static void UNIQUE_NAME(test_, __LINE__)()
