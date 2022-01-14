#ifndef ROBOT_STATICMATRIX_HPP
#define ROBOT_STATICMATRIX_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>
#include <arm_neon.h>

#include "./TypeTraits.hpp"

#include "./StaticVector/StaticVector.hpp"

template<typename Type, const size_t Rows, const size_t Columns> requires std::is_arithmetic_v<Type>
class StaticMatrix {
private:

    Type _elements[Rows * Columns]{};

    using MatrixType = StaticMatrix<Type, Rows, Columns>;

    using InVectorType = StaticVector<Type, Columns>;
    using OutVectorType = StaticVector<Type, Rows>;

    template<size_t SecondColumns>
    using InMatrixType = StaticMatrix<Type, Columns, SecondColumns>;

    template<size_t SecondColumns>
    using OutMatrixType = StaticMatrix<Type, Rows, SecondColumns>;

public:

    StaticMatrix() = default;

    StaticMatrix(const MatrixType &matrix);

    StaticMatrix(const Type *array, size_t rows);

    ~StaticMatrix() = default;

    Type *operator[](size_t index) { return &(_elements[Columns * index]); }

    const Type *operator[](size_t index) const { return &(_elements[Columns * index]); }

    [[nodiscard]] inline size_t GetRow() const { return Rows; }

    [[nodiscard]] inline size_t GetColumn() const { return Columns; }

    Type *begin() { return _elements; }

    Type *end() { return _elements + Rows * Columns; }

    MatrixType &operator=(const MatrixType &) noexcept;

    MatrixType &operator*=(Type value);

    MatrixType &operator*=(Type value) requires IsFloat32<Type>;

    MatrixType &operator/=(Type value);

    MatrixType &operator+=(Type value);

    MatrixType &operator+=(Type value) requires IsFloat32<Type>;

    template<size_t SecondColumn>
    OutMatrixType<SecondColumn> operator*(const InMatrixType<SecondColumn> &matrix) noexcept {
        StaticMatrix<Type, Rows, SecondColumn> newMatrix;

        for (size_t i = 0; i < Rows; i++) {
            for (size_t j = 0; j < Columns; j++) {
                for (size_t k = 0; k < SecondColumn; k++) {
                    newMatrix[i][k] += operator[](i)[j] * matrix[j][k];
                }
            }
        }

        return newMatrix;
    }

    OutVectorType operator*(const InVectorType &inVector) const noexcept requires IsFloat32<Type>;


    friend std::ostream &operator<<(std::ostream &stream, StaticMatrix &matrix) {
        for (ssize_t i = 0; i < Rows; i++) {
            stream << '[';
            for (ssize_t j = 0; j < Columns - 1; j++) {
                stream << matrix[i][j] << ", ";
            }
            stream << matrix[i][Columns - 1];
            stream << "]\n";
        }

        return stream;
    }
};


template<typename Type, size_t Rows, size_t Columns>
requires std::is_arithmetic_v<Type>
StaticMatrix<Type, Rows, Columns>::StaticMatrix(const StaticMatrix<Type, Rows, Columns> &matrix) {
    for (size_t i = 0; i < Rows * Columns; ++i)
        _elements[i] = matrix._elements[i];
}

template<typename Type, size_t Rows, size_t Columns>
requires std::is_arithmetic_v<Type>
StaticMatrix<Type, Rows, Columns>::StaticMatrix(const Type *array, size_t rows) {
    for (size_t i = 0; i < Columns * rows; ++i)
        _elements[i] = array[i];
}

template<typename Type, size_t Rows, size_t Columns>
requires std::is_arithmetic_v<Type>
StaticMatrix<Type, Rows, Columns> &
StaticMatrix<Type, Rows, Columns>::operator=(const StaticMatrix<Type, Rows, Columns> &matrix) noexcept {
    if (&matrix == this)
        return *this;

    for (ssize_t i = 0; i < Rows * Columns; i++)
        _elements[i] *= matrix._elements[i];

    return *this;
}

template<typename Type, size_t Rows, size_t Columns>
requires std::is_arithmetic_v<Type>
StaticMatrix<Type, Rows, Columns> &StaticMatrix<Type, Rows, Columns>::operator*=(Type value) {
    for (ssize_t i = 0; i < Rows * Columns; i++)
        _elements[i] *= value;

    return *this;
}

template<typename Type, size_t Rows, size_t Columns>
requires std::is_arithmetic_v<Type>
StaticMatrix<Type, Rows, Columns> &StaticMatrix<Type, Rows, Columns>::operator*=(Type value) requires IsFloat32<Type> {

    const size_t iterationCount = GetIterationCount<Type>(Rows * Columns);
    const size_t alignment = GetAlignment<Type>(Rows * Columns);

    auto *array = (float32x4_t *) _elements;
    for (size_t i = 0; i < iterationCount; ++i)
        array[i] = vmulq_n_f32(array[i], value);

    ///If size is 16 byte aligned
    if (alignment == 0)
        return *this;

    for (size_t i = Rows * Columns - alignment; i < Rows * Columns; ++i)
        _elements[i] *= value;

    return *this;
}

template<typename Type, size_t Rows, size_t Columns>
requires std::is_arithmetic_v<Type>
StaticMatrix<Type, Rows, Columns> &StaticMatrix<Type, Rows, Columns>::operator/=(Type value) {
    for (ssize_t i = 0; i < Rows * Columns; i++)
        _elements[i] /= value;
    return *this;
}

template<typename Type, size_t Rows, size_t Columns>
requires std::is_arithmetic_v<Type>
StaticMatrix<Type, Rows, Columns> &StaticMatrix<Type, Rows, Columns>::operator+=(Type value) {
    for (size_t i = 0; i < Rows * Columns; ++i)
        _elements[i] += value;

    return *this;
}

template<typename Type, size_t Rows, size_t Columns>
requires std::is_arithmetic_v<Type>
StaticMatrix<Type, Rows, Columns> &StaticMatrix<Type, Rows, Columns>::operator+=(Type value) requires IsFloat32<Type> {

    const size_t iterationCount = GetIterationCount<Type>(Rows * Columns);
    const size_t alignment = GetAlignment<Type>(Rows * Columns);

    Type valueArray[4] = {value, value, value, value};
    auto *values = (float32x4_t *) valueArray;

    auto *array = (float32x4_t *) _elements;
    for (size_t i = 0; i < iterationCount; ++i)
        array[i] = vaddq_f32(array[i], *values);

    ///If size is 16 byte aligned
    if (alignment == 0)
        return *this;

    for (size_t i = Rows * Columns - alignment; i < Rows * Columns; ++i)
        _elements[i] += value;

    return *this;
}


template<typename Type, size_t Rows, size_t Columns>
requires std::is_arithmetic_v<Type>
StaticVector<Type, Rows> StaticMatrix<Type, Rows, Columns>::operator*(const StaticVector<Type, Columns> &inVector)
        const noexcept requires IsFloat32<Type> {

    StaticVector<Type, Rows> outVector;

    const size_t iterationCount = GetIterationCount<Type>(Rows * Columns);
    const size_t alignment = GetAlignment<Type>(Rows * Columns);

    const auto *inVectorArray = (const float32x4_t *) &inVector[0];

    for (size_t i = 0; i < Rows; ++i) {
        const auto *matrixArray = (const float32x4_t *) operator[](i);

        for (size_t j = 0; j < iterationCount; ++j) {
            float32x4_t buf = vmulq_f32(matrixArray[j], inVectorArray[j]);
            outVector[i] += vaddvq_f32(buf);
        }

        if (alignment == 0)
            continue;

        for (size_t j = Columns - iterationCount; j < Columns; j++) {
            outVector[i] += operator[](i)[j] * inVector[j];
        }
    }

    return outVector;
}

#endif
