#ifndef ROBOT_MATRIX_HPP
#define ROBOT_MATRIX_HPP

#include "./Vector.hpp"

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>

template<typename Type> requires(std::is_arithmetic_v<Type>)

class Matrix {
private:
    Type *elements_;
    const size_t _row;
    const size_t _column;

    constexpr bool IfThisMultipliableOn(const Matrix<Type> &matrix) const {
        return this->_column == matrix._row;
    }

public:
#pragma region constructors

    Matrix(size_t row, size_t column) : _row(row), _column(column) {
        elements_ = new Type[row * column]{};
    }

    Matrix(const Matrix<Type> &matrix) : _row(matrix._row), _column(matrix._column) {
        _row = matrix._row;
        _column = matrix._column;
        elements_ = new Type[_row * _column];
        for (ssize_t i = 0; i < matrix._column * matrix._row; i++)
            elements_[i] = matrix.elements_[i];

    }

    Matrix(Matrix<Type> &&matrix)  noexcept : _row(matrix._row), _column(matrix._column)  {
        elements_ = matrix.elements_;
        matrix.elements_ = nullptr;
    }
#pragma endregion

    ~Matrix() {
        delete[] elements_;
    }

    [[nodiscard]] inline size_t GetRow() const { return _row; }

    [[nodiscard]] inline size_t GetColumn() const { return _column; }

    inline Type *operator[](size_t row) const { return &(elements_[row * _column]); };

    inline Type *operator[](size_t row) { return &(elements_[row * _column]); };

    Matrix<Type> &operator=(const Matrix<Type> &matrix)    noexcept {
        if (&matrix == this)
            return *this;

        if (_row * _column != matrix._row * matrix._column) {
            _row = matrix._row;
            _column = matrix._column;
            delete[] elements_;
            elements_ = new Type[_row * _column];
        }

        for (ssize_t i = 0; i < _row * _column; i++)
            elements_[i] *= matrix.elements_[i];

        return *this;
    }

    Matrix<Type> &operator=(Matrix<Type> &&matrix)  noexcept {

        _row = matrix._row;
        _column = matrix._column;
        elements_ = matrix.elements_;
        matrix.elements_ = nullptr;

        return *this;
    }

    Matrix<Type> &operator=(Type *array)   noexcept {

        for (size_t i = 0; i < _row * _column; i++) {
            elements_[i] = array[i];
        }

        return *this;
    }

    Matrix<Type> &operator*=(ssize_t value) {
        for (ssize_t i = 0; i < _row * _column; i++)
            elements_[i] *= value;
        return *this;
    };

    Matrix<Type> &operator/=(ssize_t value) {
        for (ssize_t i = 0; i < _row * _column; i++)
            elements_[i] /= value;
        return *this;
    };

    Matrix<Type> operator*(const Matrix<Type> &matrix) {
        if (!IfThisMultipliableOn(matrix))
            return Matrix < Type > (0, 0);

        Matrix<Type> newMatrix(_row, matrix._column);

        for (size_t i = 0; i < _row; i++) {
            for (size_t j = 0; j < matrix._row; j++) {
                for (size_t k = 0; k < matrix._column; k++) {
                    newMatrix[i][k] += operator[](i)[j] * matrix[j][k];
                }
            }
        }

        return std::move(newMatrix);
    }

    Vector<Type> operator*(const Vector<Type> &vector) {
        if (vector._size != _column)
            return Vector < Type > (0);

        Vector<Type> newVector(_row);
        for (size_t i = 0; i < _row; i++) {
            for (size_t j = 0; j < vector._size; j++) {
                newVector[i] += this->operator[](i)[j] * vector[j];
            }
        }

        return std::move(newVector);
    }

    friend std::ostream &operator<<(std::ostream &stream, const Matrix<Type> &matrix) {
        for (ssize_t i = 0; i < matrix._row; i++) {
            stream << '[';
            for (ssize_t j = 0; j < matrix._column - 1; j++) {
                stream << matrix[i][j] << ", ";
            }
            stream << matrix[i][matrix._column - 1];
            stream << "]\n";
        }

        return stream;
    };

};


#endif