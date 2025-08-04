#pragma once

#include <compare>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace cn::math
{

template <typename scalar_type, std::size_t count>
struct vector
{
    scalar_type values[count]{};

    constexpr vector() = default;

    template <typename... args_type>
        requires(sizeof...(args_type) == count && (std::is_convertible_v<args_type, scalar_type> && ...))
    constexpr vector(args_type... args)
        : values{static_cast<scalar_type>(args)...}
    {
    }

    constexpr scalar_type&       operator[](std::size_t index) { return values[index]; }
    constexpr const scalar_type& operator[](std::size_t index) const { return values[index]; }

    constexpr auto operator<=>(const vector&) const = default;

    constexpr vector operator+(const vector& other) const
    {
        vector result;
        for(std::size_t i = 0; i < count; ++i)
        {
            result.values[i] = values[i] + other.values[i];
        }
        return result;
    }

    constexpr vector operator*(scalar_type scalar) const
    {
        vector result;
        for(std::size_t i = 0; i < count; ++i)
        {
            result.values[i] = values[i] * scalar;
        }
        return result;
    }

    constexpr vector& operator*=(const vector& other)
    {
        for(std::size_t i = 0; i < count; ++i)
        {
            values[i] *= other[i];
        }
        return *this;
    }

    constexpr vector& operator+=(const vector& other)
    {
        for(std::size_t i = 0; i < count; ++i)
        {
            values[i] += other.values[i];
        }
        return *this;
    }

    constexpr scalar_type*       data() { return values; }
    constexpr const scalar_type* data() const { return values; }
};

} // namespace cn::math
