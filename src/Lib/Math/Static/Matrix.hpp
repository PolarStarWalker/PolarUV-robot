#ifndef ROBOT_STATICMATRIX_HPP
#define ROBOT_STATICMATRIX_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>
#include <arm_neon.h>

#include "Math/TypeTraits.hpp"

#include "Math/Static/Vector.hpp"

namespace stc {
    template<typename Type, const size_t Rows, const size_t Columns> requires std::is_arithmetic_v<Type>
    struct Matrix {
    private:

        using Matrix_t = Matrix<Type, Rows, Columns>;

        using InVector_t = Vector<Vertical, Type, Columns>;
        using OutVector_t = Vector<Vertical, Type, Rows>;

        template<size_t SecondColumns>
        using InMatrix_t = Matrix<Type, Columns, SecondColumns>;

        template<size_t SecondColumns>
        using OutMatrix_t = Matrix<Type, Rows, SecondColumns>;

        using Row_t = HorizontalVector<Type, Columns>;

        Row_t rows_[Rows]{};

    public:

        Matrix() = default;

        Matrix(const Matrix_t &matrix) = default;

        Row_t &operator[](size_t index) &{
#ifdef DEBUG
            if (index >= Rows)
                throw std::out_of_range("index out of range");
#endif
            return rows_[index];
        }

        const Row_t &operator[](size_t index) const &{
#ifdef DEBUG
            if (index >= Rows)
                throw std::out_of_range("index out of range");
#endif

            return rows_[index];
        }

        [[nodiscard]] inline size_t GetRow() const { return Rows; }

        [[nodiscard]] inline size_t GetColumn() const { return Columns; }

        Row_t *begin() { return rows_; }

        Row_t *end() { return rows_ + Rows; }

        Matrix_t &operator*=(Type value);

        Matrix_t &operator*=(Type value) requires IsFloat32<Type>;

        Matrix_t &operator/=(Type value);

        Matrix_t &operator+=(Type value);

        Matrix_t &operator+=(Type value) requires IsFloat32<Type>;

        template<size_t SecondColumn>
        OutMatrix_t<SecondColumn> operator*(const InMatrix_t<SecondColumn> &matrix) const noexcept {
            Matrix<Type, Rows, SecondColumn> newMatrix;

            for (size_t i = 0; i < Rows; i++) {
                for (size_t j = 0; j < Columns; j++) {
                    for (size_t k = 0; k < SecondColumn; k++) {
                        newMatrix[i][k] += operator[](i)[j] * matrix[j][k];
                    }
                }
            }

            return newMatrix;
        }

        OutVector_t operator*(const InVector_t &inVector) const noexcept requires IsFloat32<Type>;

        friend std::ostream &operator<<(std::ostream &stream, const Matrix_t &matrix) {

            for (size_t i = 0; i < Rows - 1; ++i) {
                stream << matrix.rows_[i] << '\n';
            }

            stream << matrix.rows_[Rows - 1];

            return stream;
        };
    };

    template<typename Type, size_t Rows, size_t Columns>
    requires std::is_arithmetic_v<Type>
    Matrix<Type, Rows, Columns> &Matrix<Type, Rows, Columns>::operator*=(Type value) {

        auto *array = this->operator[](0)[0];

        for (ssize_t i = 0; i < Rows * Columns; i++)
            array[i] *= value;

        return *this;
    }

    template<typename Type, size_t Rows, size_t Columns>
    requires std::is_arithmetic_v<Type>
    Matrix<Type, Rows, Columns> &Matrix<Type, Rows, Columns>::operator*=(Type value) requires IsFloat32<Type> {

        const size_t iterationCount = GetIterationCount<Type>(Rows * Columns);
        const size_t alignment = GetAlignment<Type>(Rows * Columns);

        auto *array = (float32x4_t *) rows_->begin();
        for (size_t i = 0; i < iterationCount; ++i)
            array[i] = vmulq_n_f32(array[i], value);

        ///If size is 16 byte aligned
        if (alignment == 0)
            return *this;

        for (size_t i = Rows * Columns - alignment; i < Rows * Columns; ++i)
            rows_[i] *= value;

        return *this;
    }

    template<typename Type, size_t Rows, size_t Columns>
    requires std::is_arithmetic_v<Type>
    Matrix<Type, Rows, Columns> &Matrix<Type, Rows, Columns>::operator/=(Type value) {

        auto *array = this->operator[](0)[0];

        for (ssize_t i = 0; i < Rows * Columns; i++)
            array[i] /= value;

        return *this;
    }

    template<typename Type, size_t Rows, size_t Columns>
    requires std::is_arithmetic_v<Type>
    Matrix<Type, Rows, Columns> &Matrix<Type, Rows, Columns>::operator+=(Type value) {

        auto *array = this->operator[](0)[0];

        for (size_t i = 0; i < Rows * Columns; ++i)
            array[i] += value;

        return *this;
    }

    template<typename Type, size_t Rows, size_t Columns>
    requires std::is_arithmetic_v<Type>
    Matrix<Type, Rows, Columns> &Matrix<Type, Rows, Columns>::operator+=(Type value) requires IsFloat32<Type> {

        const size_t iterationCount = GetIterationCount<Type>(Rows * Columns);
        const size_t alignment = GetAlignment<Type>(Rows * Columns);

        Type valueArray[4] = {value, value, value, value};
        auto *values = (float32x4_t *) valueArray;

        auto *array = (float32x4_t *) rows_;
        for (size_t i = 0; i < iterationCount; ++i)
            array[i] = vaddq_f32(array[i], *values);

        ///If size is 16 byte aligned
        if (alignment == 0)
            return *this;

        for (size_t i = Rows * Columns - alignment; i < Rows * Columns; ++i)
            rows_[i] += value;

        return *this;
    }


    template<typename T, size_t Rows, size_t Columns>
    requires std::is_arithmetic_v<T>
    VerticalVector<T, Rows> Matrix<T, Rows, Columns>::operator*(const VerticalVector<T, Columns> &inVector)
    const noexcept requires IsFloat32<T> {

        VerticalVector<T, Rows> outVector;

        for (size_t i = 0; i < Rows; ++i) {
            outVector[i] = rows_[i] * inVector;
        }

        return outVector;
    }

    template<typename Type, const size_t Rows, const size_t Columns>
    requires std::is_arithmetic_v<Type>
    std::ostream &operator<<(std::ostream &stream, const Matrix<Type, Rows, Columns> &matrix) {

        for (size_t i = 0; i < Rows - 1; ++i) {
            stream << matrix.rows_[i] << '\n';
        }

        stream << matrix.rows_[Rows - 1];

        return stream;
    }

}

#endif
