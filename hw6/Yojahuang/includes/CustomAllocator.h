#ifndef CUSTOMALLOCATOR_H_INCLUDED
#define CUSTOMALLOCATOR_H_INCLUDED

#include <limits>
#include <cstdlib>

struct _AllocatedCount
{
    std::size_t allocated;
    std::size_t deallocated;

    void increase(std::size_t sz)
    {
        this->allocated += sz;
    }

    void decrease(std::size_t sz)
    {
        this->deallocated += sz;
    }
};

template <class T>
class CustomAllocator
{
private:
    static struct _AllocatedCount count;

public:
    using value_type = T;
    static size_t bytes()
    {
        return count.allocated - count.deallocated;
    }
    static size_t allocated()
    {
        return count.allocated;
    }
    static size_t deallocated()
    {
        return count.deallocated;
    }
    CustomAllocator() noexcept = default;

    T *allocate(std::size_t sz)
    {
        if (sz > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t n_bytes = sz * sizeof(T);
        T *ptr = static_cast<T *>(std::malloc(n_bytes));
        if (ptr)
        {
            count.increase(n_bytes);
            return ptr;
        }
        throw std::bad_alloc();
    }
    void deallocate(T *ptr, std::size_t sz) noexcept
    {
        std::free(ptr);

        const std::size_t n_bytes = sz * sizeof(T);
        count.decrease(n_bytes);
    }
};

template <class T>
struct _AllocatedCount CustomAllocator<T>::count =
{
    .allocated = 0,
    .deallocated = 0
};

#endif