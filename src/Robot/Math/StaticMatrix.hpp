#ifndef ROBOT_STATICMATRIX_HPP
#define ROBOT_STATICMATRIX_HPP
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>

#include "./StaticVector.hpp"

template<typename Type, const size_t Rows, const size_t Columns> requires(std::is_arithmetic_v<Type>)
class StaticMatrix {
private:
    Type _elements[Rows * Columns]{};

public:

    StaticMatrix() = default;

    StaticMatrix(const StaticMatrix<Type, Rows, Columns> &matrix) {
        StaticMatrix<Type, Rows, Columns> newMatrix;

        for (size_t i = 0; i < Rows * Columns; ++i)
            _elements[i] = matrix._elements[i];

        return newMatrix;
    }

    ~StaticMatrix() = default;

    Type* operator[](size_t index){ return &(_elements[Rows * index]);}
    const Type* operator[](size_t index) const{return &(_elements[Rows * index]);}

    inline size_t GetRow() const { return Rows; }

    inline size_t GetColumn() const { return Columns; }

    StaticMatrix<Type, Rows, Columns> &operator=(const StaticMatrix<Type, Rows, Columns> &matrix)    noexcept {
        if (&matrix == this)
            return *this;

        for (ssize_t i = 0; i < Rows * Columns; i++)
            _elements[i] *= matrix._elements[i];

        return *this;
    }

    StaticMatrix<Type, Rows, Columns> &operator=(Type *array)  noexcept {

        for (size_t i = 0; i < Rows * Columns; i++) {
            _elements[i] = array[i];
        }

        return *this;
    }

    StaticMatrix<Type, Rows, Columns> &operator*=(ssize_t value) {
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
    StaticMatrix<Type, Rows, secondColumn> operator*(const StaticMatrix<Type, Columns, secondColumn> &matrix) noexcept{
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

    StaticVector<Type, Rows> operator*(const StaticVector<Type, Columns>& vector) noexcept{

        StaticVector<Type, Rows> newVector;

        for(size_t i = 0; i < Rows; ++i){
            for(size_t j = 0; j < Columns; ++j){
                newVector[i] += operator[](i)[j] + vector._elements[j];
            }
        }

        return newVector;
    }

    friend std::ostream &operator<<(std::ostream &stream, const StaticMatrix<Type, Columns, Rows> &matrix) {
        for (ssize_t i = 0; i < Rows; i++) {
            stream << '[';
            for (ssize_t j = 0; j < Columns - 1; j++) {
                stream << matrix[i][j] << ", ";
            }
            stream << matrix[i][Columns- 1];
            stream << "]\n";
        }

        return stream;
    };

};

#endif
