#ifndef EXCEPTION_GUARD_H
#define EXCEPTION_GUARD_H
#include <functional>
class ExceptionGuard
{
    using FunctionType = std::function<void()>;

public:
    explicit ExceptionGuard(FunctionType fn)
        : rollback(fn)
        , active(true)
    {
    }
    ~ExceptionGuard()
    {
        if (active)
        {
            rollback();
        }
    }
    void release()
    {
        active = false;
    }

private:
    bool         active;
    FunctionType rollback;
};
#endif
