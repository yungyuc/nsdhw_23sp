#include "CustomAllocator.h"
#include "iostream"

template class CustomAllocator<double>;

template <class T>
size_t CustomAllocator<T>::m_bytes_allocated = 0;

template <class T>
size_t CustomAllocator<T>::m_bytes_deallocated = 0;

template <class T>
template <class U>
CustomAllocator<T>::CustomAllocator(const CustomAllocator<U> &) noexcept {}

template <class T>
T *CustomAllocator<T>::allocate(size_t n)
{
    m_bytes_allocated += n * sizeof(T);
    return static_cast<T *>(::operator new(n * sizeof(T)));
}

template <class T>
void CustomAllocator<T>::deallocate(T *p, size_t n)
{
    m_bytes_deallocated += n * sizeof(T);
    ::operator delete(p);
}

template <class T>
size_t CustomAllocator<T>::bytes() { return m_bytes_allocated - m_bytes_deallocated; }

template <class T>
size_t CustomAllocator<T>::allocated() { return m_bytes_allocated; }

template <class T>
size_t CustomAllocator<T>::deallocated() { return m_bytes_deallocated; }
