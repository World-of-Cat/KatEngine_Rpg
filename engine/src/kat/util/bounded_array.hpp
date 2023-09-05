#pragma once

#include <cinttypes>
#include <cassert>

namespace kat::util {
    template<typename T, size_t max>
    class bounded_array {
    public:

        inline bounded_array() {};

        inline const T& operator[](const size_t& index) const { assert(index < max); return array[index]; };
        inline T& operator[](const size_t& index) { assert(index < max); return array[index]; };

        inline T* at(const size_t& index) { assert(index < max); return &array[index]; };
        inline const T* at(const size_t& index) const { assert(index < max); return &array[index]; };

        constexpr static size_t max_size() { return max; };

        inline const size_t& size() const noexcept { return real_size; };

        inline bool try_append(const T& value) {
            if (real_size < max) {
                array[real_size++] = value;
                return true;
            }
            return false;
        };

        inline bool try_append(T&& value) {
            if (real_size < max) {
                array[real_size++] = value;
                return true;
            }
            return false;
        };

    private:
        T array[max];
        size_t real_size = 0;
    };
}