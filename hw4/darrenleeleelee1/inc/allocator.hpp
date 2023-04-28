#include <cstdlib>
#include <new>
#include <memory>
#include <limits>
#include <atomic>
#include <vector>
template <class T>
class CustomAllocator {

    public:
        using value_type = T;
        CustomAllocator() = default;

        T* allocate(size_t n)
        {
            if (n > std::numeric_limits<size_t>::max()/sizeof(T))
                throw std::bad_alloc();

            const size_t bytes = n*sizeof(T);
            T* ret = static_cast<T*>(malloc(bytes));

            if (ret)
            {
                m_allocated += bytes;
                return ret;
            }
            else
                throw std::bad_alloc();
        }

        void deallocate(T* p, size_t n) noexcept
        {
            free(p);
            m_deallocated += n*sizeof(T);
        }
        // Comparison operators
        bool operator==(const CustomAllocator& other) const noexcept {
            return this == &other;
        }

        bool operator!=(const CustomAllocator& other) const noexcept {
            return !(*this == other);
        }

        static size_t bytes() { return m_allocated - m_deallocated; }
        static size_t allocated() { return m_allocated; }
        static size_t deallocated() { return m_deallocated; }

    private:
        static size_t m_allocated, m_deallocated;
};