#ifndef CUSTOM_ALLOCATOR_H_INCLUDED
#define CUSTOM_ALLOCATOR_H_INCLUDED

template <typename T>
class CustomAllocator
{
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    CustomAllocator() noexcept = default;

    template <class U>
    CustomAllocator(const CustomAllocator<U> &) noexcept {}

    T *allocate(std::size_t n)
    {
        m_bytes_allocated += n * sizeof(T);
        return static_cast<T *>(::operator new(n * sizeof(T)));
    }

    void deallocate(T *p, std::size_t n)
    {
        m_bytes_deallocated += n * sizeof(T);
        ::operator delete(p);
    }

    std::size_t bytes() const noexcept { return m_bytes_allocated - m_bytes_deallocated; }
    std::size_t allocated() const noexcept { return m_bytes_allocated; }
    std::size_t deallocated() const noexcept { return m_bytes_deallocated; }

private:
    std::size_t m_bytes_allocated{0};
    std::size_t m_bytes_deallocated{0};
};

#endif