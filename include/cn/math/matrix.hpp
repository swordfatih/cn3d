#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cn/math/vector.hpp>
#include <cstddef>

namespace cn::math
{

template <typename type, std::size_t rows, std::size_t cols>
struct matrix
{
    static constexpr std::size_t row_count = rows;
    static constexpr std::size_t column_count = cols;

    using value_type = type;
    using storage_type = std::array<type, rows * cols>;

    storage_type elements{};

    constexpr matrix() = default;

    constexpr explicit matrix(type identity_scalar)
    {
        static_assert(rows == cols, "Identity only defined for square matrices");
        for(std::size_t i = 0; i < rows; ++i)
            (*this)(i, i) = identity_scalar;
    }

    [[nodiscard]] constexpr type& operator()(std::size_t row, std::size_t column)
    {
        return elements[column * rows + row];
    }

    [[nodiscard]] constexpr const type& operator()(std::size_t row, std::size_t column) const
    {
        return elements[column * rows + row];
    }

    [[nodiscard]] constexpr type* data() noexcept
    {
        return elements.data();
    }

    [[nodiscard]] constexpr const type* data() const noexcept
    {
        return elements.data();
    }

    constexpr void fill(type value)
    {
        std::fill(elements.begin(), elements.end(), value);
    }

    [[nodiscard]] constexpr bool operator==(const matrix& other) const
    {
        return elements == other.elements;
    }

    [[nodiscard]] constexpr bool operator!=(const matrix& other) const
    {
        return !(*this == other);
    }

    static constexpr matrix identity(type identity_scalar = type(1))
    {
        static_assert(rows == cols, "Identity only defined for square matrices");
        matrix result;
        result.fill(type(0)); // Fill with 0
        for(std::size_t i = 0; i < rows; ++i)
        {
            result(i, i) = identity_scalar; // Set diagonal to identity_scalar (1 by default)
        }
        return result;
    }

    static constexpr matrix translation(const vector<type, 3>& position)
    {
        static_assert(rows == cols, "Translation matrix requires square matrix");
        matrix result = identity();
        result(0, 3) = position[0];
        result(1, 3) = position[1];
        result(2, 3) = position[2];
        return result;
    }

    static constexpr matrix scaling(const vector<type, 3>& scale)
    {
        static_assert(rows == cols, "Scaling matrix requires square matrix");
        matrix result = identity();
        result(0, 0) = scale[0];
        result(1, 1) = scale[1];
        result(2, 2) = scale[2];
        return result;
    }

    static constexpr matrix rotation_euler(const vector<type, 3>& euler_angles)
    {
        static_assert(rows == cols, "Rotation matrix requires square matrix");

        type cx = std::cos(euler_angles[0]);
        type sx = std::sin(euler_angles[0]);
        type cy = std::cos(euler_angles[1]);
        type sy = std::sin(euler_angles[1]);
        type cz = std::cos(euler_angles[2]);
        type sz = std::sin(euler_angles[2]);

        matrix result = identity();

        result(0, 0) = cy * cz;
        result(0, 1) = -cy * sz;
        result(0, 2) = sy;

        result(1, 0) = sx * sy * cz + cx * sz;
        result(1, 1) = -sx * sy * sz + cx * cz;
        result(1, 2) = -sx * cy;

        result(2, 0) = -cx * sy * cz + sx * sz;
        result(2, 1) = cx * sy * sz + sx * cz;
        result(2, 2) = cx * cy;

        return result;
    }

    template <std::size_t other_cols>
    constexpr matrix<type, rows, other_cols> operator*(const matrix<type, cols, other_cols>& other) const
    {
        matrix<type, rows, other_cols> result{};

        for(std::size_t i = 0; i < rows; ++i)
        {
            for(std::size_t j = 0; j < other_cols; ++j)
            {
                result(i, j) = 0;
                for(std::size_t k = 0; k < cols; ++k)
                {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }

        return result;
    }
};

} // namespace cn::math
