#ifndef ROBOT_STATICMATRIX_HPP
#define ROBOT_STATICMATRIX_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>
#include <arm_neon.h>

#include "./StaticVector.hpp"

template<typename Type, const size_t Rows, const size_t Columns> requires(std::is_arithmetic_v<Type>)
class StaticMatrix {
private:
    Type _elements[Rows * Columns]{};

public:

    StaticMatrix() = default;

    StaticMatrix(const StaticMatrix<Type, Rows, Columns> &matrix) {
        for (size_t i = 0; i < Rows * Columns; ++i)
            _elements[i] = matrix._elements[i];

    }

    StaticMatrix(const Type array[ Rows * Columns]) {
        for (size_t i = 0; i < Rows * Columns; ++i)
            _elements[i] = array[i];

    }

    ~StaticMatrix() = default;

    Type *operator[](size_t index) { return &(_elements[Columns * index]); }

    const Type *operator[](size_t index) const { return &(_elements[Columns * index]); }

    inline size_t GetRow() const { return Rows; }

    inline size_t GetColumn() const { return Columns; }

    StaticMatrix<Type, Rows, Columns> &operator=(const StaticMatrix<Type, Rows, Columns> &matrix) noexcept {
        if (&matrix == this)
            return *this;

        for (ssize_t i = 0; i < Rows * Columns; i++)
            _elements[i] *= matrix._elements[i];

        return *this;
    }

    StaticMatrix<Type, Rows, Columns> &operator=(Type *array) noexcept {

        for (size_t i = 0; i < Rows * Columns; i++) {
            _elements[i] = array[i];
        }

        return *this;
    }

    StaticMatrix<Type, Rows, Columns> &operator*=(Type value) {

        const size_t bytesCount = Rows * Columns * sizeof(Type);
        const size_t interationCount = bytesCount / 16;
        const size_t aligment = (bytesCount % 16) / sizeof(Type);

        ///If using float32_t
        if (std::is_floating_point_v<Type> && sizeof(Type) == 4) {

            for (size_t i = 0; i < interationCount; ++i) {
                *((float32x4_t *) &(_elements[i * sizeof(Type)])) =
                        vmulq_n_f32(*((float32x4_t *) &(_elements[i * sizeof(Type)])), value);
            }

            ///If size is 16 byte aligned
            if (aligment == 0)
                return *this;

            for (size_t i = Rows * Columns - aligment; i < Rows * Columns; ++i) {
                _elements[i] *= value;
            }

            return *this;
        }

        for (ssize_t i = 0; i < Rows * Columns; i++)
            _elements[i] *= value;

        return *this;
    };

    StaticMatrix<Type, Rows, Columns> &operator/=(ssize_t value) {
        for (ssize_t i = 0; i < Rows * Columns; i++)
            _elements[i] /= value;
        return *this;
    };

    template<size_t secondColumn>
    StaticMatrix<Type, Rows, secondColumn> operator*(const StaticMatrix<Type, Columns, secondColumn> &matrix) noexcept {
        StaticMatrix<Type, Rows, secondColumn> newMatrix;

        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < Columns; j++) {
                for (size_t k = 0; k < secondColumn; k++) {
                    newMatrix[i][k] += operator[](i)[j] * matrix[j][k];
                }
            }
        }

        return newMatrix;
    }

    StaticVector<Type, Rows> operator*(const StaticVector<Type, Columns> &vector) const noexcept {

        StaticVector<Type, Rows> newVector;

        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Columns; ++j) {
                newVector[i] += operator[](i)[j] * vector[j];
            }
        }

        return newVector;
    }

    StaticMatrix(const Type * array, size_t rows) {
        for (size_t i = 0; i < Columns * rows; ++i)
            _elements[i] = array[i];
    }

    friend std::ostream &operator<<(std::ostream &stream, const StaticMatrix<Type, Rows, Columns> &matrix) {
        for (ssize_t i = 0; i < Rows; i++) {
            stream << '[';
            for (ssize_t j = 0; j < Columns - 1; j++) {
                stream << matrix[i][j] << ", ";
            }
            stream << matrix[i][Columns - 1];
            stream << "]\n";
        }

        return stream;
    };

};

#endif
