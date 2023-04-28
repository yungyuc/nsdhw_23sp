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
            // counter.increase(bytes);
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
        // counter.decrease(bytes);
    }

    std::size_t bytes() const { return alloc - dealloc; }
    std::size_t allocated() const { return alloc; }
    std::size_t deallocated() const { return dealloc; }

private:
    std::size_t alloc = 0;
    std::size_t dealloc = 0;

}; /* end struct MyAllocator */

