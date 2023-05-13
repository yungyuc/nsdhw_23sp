#ifndef CUSTOM_ALLOCATOR_H_INCLUDED
#define CUSTOM_ALLOCATOR_H_INCLUDED

#include <cstddef>

template <typename T>
class CustomAllocator
{
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    CustomAllocator() noexcept = default;

    template <class U>
    CustomAllocator(const CustomAllocator<U> &) noexcept;

    T *allocate(size_t n);

    void deallocate(T *p, size_t n);

    static size_t bytes();
    static size_t allocated();
    static size_t deallocated();

private:
    static size_t m_bytes_allocated;
    static size_t m_bytes_deallocated;
};

#endif