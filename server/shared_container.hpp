#ifndef PROJECT_SHARED_ARRAY_HPP
#define PROJECT_SHARED_ARRAY_HPP
#include "shared_memory.hpp"
#include <cstring>
#include <stdexcept>
#include <functional>

namespace shared_memory {
    template <typename T>
    class array {
        T* m_data = nullptr;
        const size_t size_type = sizeof(T);
        const size_t m_capacity;
        size_t m_length;

    protected:
        void move(size_t from, size_t to) {
            auto move_size = (length() - from) * size_type;
            std::memmove(m_data + to, m_data + from, move_size);
        }

    public:
        using delegate = std::function<bool(const T&)>;

        static array<T>& create(size_t capacity) {
            return allocator::allocate_object_ref<array<T>>(capacity);
        }

        array() = delete;

        explicit array(size_t capacity)
            : m_capacity{capacity}, m_length{} {
            m_data = static_cast<T*>(allocator::allocate(capacity * size_type));
        }

        decltype(m_capacity) capacity() const {
            return m_capacity;
        }

        decltype(m_length) length() const {
            return m_length;
        }

        bool is_empty() const {
            return length() == 0;
        }

        bool is_full() const {
            return length() == capacity();
        }

        T& at(size_t id) {
            if (id < length())
                return m_data[id];
            throw std::out_of_range("Allow to " + std::to_string(id) + " with length " + std::to_string(length()));
        }

        T&operator[](size_t id) {
            return at(id);
        }

        const T&operator[] (size_t id) const {
            return at(id);
        }

        void insert(size_t pos, const T& val) {
            if (is_full())
                return;
            pos = pos > length() ? length() : pos;
            move(pos, pos + 1);
            ++m_length;
            at(pos) = val;
        }

        void erase(size_t pos) {
            if (is_empty() || pos >= length())
                return;
            move(pos + 1, pos);
            --m_length;
        }

        void push_back(const T& val) {
            insert(length(), val);
        }

        void push_front(const T& val) {
            insert(0, val);
        }

        void pop_front() {
            erase(0);
        }

        void pop_back() {
            --m_length;
        }

        template <typename ...Args>
        const T& emplace_back(Args... args) {
            auto id = m_length++;
            auto* object = new (&m_data[id]) T{ args... };//T{ std::forward<Args>(args)... };
            return *object;
        }

        size_t find(const delegate& func_delegate) {
            for (size_t id = 0; id < length(); ++id)
                if (func_delegate(at(id)))
                    return id;
            return (size_t)-1;
        }

        size_t find(const T& val) const {
            return find([&val](const T& right){ return val == right; });
        }

    }; //class array

    //TODO: map
} //namespace shared_memory

#endif //PROJECT_SHARED_ARRAY_HPP
