#ifndef PROJECT_SHARED_MEMORY_HPP
#define PROJECT_SHARED_MEMORY_HPP

#include <sys/mman.h>

namespace shared_memory {
    class allocator {
    public:
        static void* allocate(size_t size) {
            const int protection = PROT_READ | PROT_WRITE;
            const int visibility = MAP_ANONYMOUS | MAP_SHARED;
            return mmap(nullptr, size, protection, visibility, 0, 0);
        }

        template<typename T, typename ...Args>
        static T* allocate_object(Args... args) {
            auto alloc_ptr = allocate(sizeof(T));
            auto object_ptr = new (alloc_ptr) T { args... };
            return object_ptr;
        }

        template<typename T, typename ...Args>
        static T& allocate_object_ref(Args... args) {
            return *allocate_object<T>(args...);
        }
    };
}

#endif //PROJECT_SHARED_MEMORY_HPP
