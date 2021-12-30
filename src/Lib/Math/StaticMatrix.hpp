#ifndef ROBOT_STATICMATRIX_HPP
#define ROBOT_STATICMATRIX_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>
#include <arm_neon.h>
#include <variant>

#include "./StaticVector/StaticVector.hpp"

template<typename Type, const size_t Rows, const size_t Columns> requires(std::is_arithmetic_v<Type>)
class StaticMatrix {
private:


    //std::variant<Type*, float32x4_t*, float64x1x4_t*> _elements[Rows * Columns];

    Type _elements[Rows * Columns]{};

public:

    StaticMatrix() = default;

    StaticMatrix(const StaticMatrix<Type, Rows, Columns> &matrix) {
        for (size_t i = 0; i < Rows * Columns; ++i)
            _elements[i] = matrix._elements[i];

    }

    StaticMatrix(const Type array[Rows * Columns]) {
        for (size_t i = 0; i < Rows * Columns; ++i)
            _elements[i] = array[i];

    }

    StaticMatrix(const Type *array, size_t rows) {
        for (size_t i = 0; i < Columns * rows; ++i)
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

            auto *array = (float32x4_t *) _elements;
            for (size_t i = 0; i < interationCount; ++i) {
                array[i] = vmulq_n_f32(array[i], value);
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

    StaticMatrix<Type, Rows, Columns> &operator+=(Type value) {

        const size_t bytesCount = Rows * Columns * sizeof(Type);
        const size_t interationCount = bytesCount / 16;
        const size_t aligment = (bytesCount % 16) / sizeof(Type);

        ///If using float32_t
        if (std::is_floating_point_v<Type> && sizeof(Type) == 4) {

            Type valueArray[4] = {value, value, value, value};
            auto *values = (float32x4_t *) valueArray;

            auto *array = (float32x4_t *) _elements;
            for (size_t i = 0; i < interationCount; ++i) {
                array[i] = vaddq_f32(array[i], *values);
            }

            ///If size is 16 byte aligned
            if (aligment == 0)
                return *this;

            for (size_t i = Rows * Columns - aligment; i < Rows * Columns; ++i) {
                _elements[i] += value;
            }

            return *this;
        }
        ///other Types

        for (size_t i = 0; i < Rows * Columns; ++i) {
            _elements[i] += value;
        }

        return *this;
    }

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

    StaticVector<Type, Rows> operator*(const StaticVector<Type, Columns> &inVector) const noexcept {

        StaticVector<Type, Rows> outVector;

        if (std::is_floating_point_v<Type> && sizeof(Type) == 4) {

            const size_t bytesCount = Columns * sizeof(Type);
            const size_t interationCount = bytesCount / 16;
            const size_t aligment = (bytesCount % 16) / sizeof(Type);

            const auto *inVectorArray = (const float32x4_t *) &inVector[0];

            for (size_t i = 0; i < Rows; ++i) {
                const auto *matrixArray = (const float32x4_t *) operator[](i);

                for (size_t j = 0; j < interationCount; ++j) {
                    float32x4_t buf = vmulq_f32(matrixArray[j], inVectorArray[j]);
                    outVector[i] += vaddvq_f32(buf);
                }

                if (aligment == 0)
                    continue;

                for (size_t j = Columns - interationCount; j < Columns; j++) {
                    outVector[i] += operator[](i)[j] * inVector[j];
                }
            }

            return outVector;
        }

        for (size_t i = 0; i < Rows; ++i) {
            for (size_t j = 0; j < Columns; ++j) {
                outVector[i] += operator[](i)[j] * inVector[j];
            }
        }

        return outVector;
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
