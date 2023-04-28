#include <limits>
#include <memory>

template <class T>
struct MyAllocator
{
public:
    using value_type = T;

    MyAllocator() = default;

    T * allocate(std::size_t n)
    {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        {
            throw std::bad_alloc();
        }
        const std::size_t bytes = n*sizeof(T);
        T * p = static_cast<T *>(std::malloc(bytes));
        if (p)
        {
            alloc += bytes;
            return p;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void deallocate(T* p, std::size_t n) noexcept
    {
        std::free(p);

        const std::size_t bytes = n*sizeof(T);
        dealloc += bytes;
    }

    static std::size_t bytes() { return  alloc - dealloc; }
    static std::size_t allocated() { return  alloc; }
    static std::size_t deallocated() { return dealloc; }

private:

    static std::size_t alloc;
    static std::size_t dealloc;

};

