// #include "test.hpp"

// TEST("Weird T")
// {
//     struct WeirdT
//     {
//         int*        ptr;
//         std::string name;

//         WeirdT()
//             : ptr(new int(42))
//             , name("default")
//         {
//             std::cout << "Default ctor\n";
//         }

//         WeirdT(WeirdT const& other)
//             : ptr(new int(*other.ptr))
//             , name(other.name)
//         {
//             std::cout << "Copy ctor\n";
//         }

//         WeirdT(WeirdT&& other) noexcept
//             : ptr(other.ptr)
//             , name(std::move(other.name))
//         {
//             other.ptr = nullptr;
//             std::cout << "Move ctor\n";
//         }

//         ~WeirdT()
//         {
//             if (ptr)
//             {
//                 std::cout << "Deleting ptr\n";
//                 delete ptr;
//             }
//         }

//         WeirdT& operator=(WeirdT const& other)
//         {
//             if (this != &other)
//             {
//                 delete ptr;
//                 ptr  = new int(*other.ptr);
//                 name = other.name;
//             }
//             return *this;
//         }

//         WeirdT& operator=(WeirdT&& other) noexcept
//         {
//             if (this != &other)
//             {
//                 delete ptr;
//                 ptr       = other.ptr;
//                 other.ptr = nullptr;
//                 name      = std::move(other.name);
//             }
//             return *this;
//         }
//     };
//     vega::vector<WeirdT> v;
//     v.reserve(3);

//     std::cout << "Pushing first element\n";
//     v.push_back(WeirdT());

//     std::cout << "Pushing second element\n";
//     v.push_back(WeirdT());

//     std::cout << "Popping last element\n";
//     v.pop_back();

//     std::cout << "Done\n";
// }

// TEST("TrapT")
// {
//     struct TrapT
//     {
//         int* ptr;

//         TrapT()
//         {
//             ptr = new int(42);
//             std::cout << "Default ctor\n";
//         }

//         TrapT(TrapT const& other)
//         {
//             ptr = new int(*other.ptr);
//             std::cout << "Copy ctor\n";
//         }

//         TrapT(TrapT&& other) noexcept
//         {
//             ptr       = other.ptr;
//             other.ptr = nullptr;
//             std::cout << "Move ctor\n";
//         }

//         ~TrapT()
//         {
//             if (ptr)
//             {
//                 std::cout << "Deleting ptr\n";
//                 delete ptr;
//             }
//             else
//             {
//                 std::cout << "Null ptr, nothing to delete\n";
//             }
//         }
//     };
//     vega::vector<TrapT> a;
//     a.push_back(TrapT{}); // triggers default ctor
//     a.push_back(TrapT{}); // triggers default ctor + maybe move
//     a.resize(10);         // <-- unsafe if memset blindly used
//     a.pop_back();
// }
