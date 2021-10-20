#ifndef ROBOT_MATRIX_HPP
#define ROBOT_MATRIX_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>

template<typename Type>
requires(std::is_arithmetic_v<Type>)
class Matrix {
public:
    Matrix(size_t row, size_t column) {
        _elements = new Type[row * column]{};
        _row = row;
        _column = column;
    }

    Matrix(const Matrix<Type> &matrix) {
        _row = matrix._row;
        _column = matrix._column;
        _elements = new Type[_row * _column];
        for (ssize_t i = 0; i < matrix._column * matrix._row; i++)
            _elements[i] = matrix._elements[i];

    }

    Matrix(Matrix<Type> &&matrix) noexcept {
        _row = matrix._row;
        _column = matrix._column;
        _elements = matrix._elements;
        matrix._elemets = nullptr;
    }

    ~Matrix() {
        delete[] _elements;
    }

    inline Type *operator[](size_t row) const { return &(_elements[row * _column]); };

    inline Type *operator[](size_t row) { return &(_elements[row * _column]); };

    Matrix<Type> &operator=(const Matrix<Type> &matrix) noexcept {
        if (&matrix == this)
            return *this;

        if (_row * _column != matrix._row * matrix._column) {
            _row = matrix._row;
            _column = matrix._column;
            delete[] _elements;
            _elements = new Type[_row * _column];
        }

        for (ssize_t i = 0; i < _row * _column; i++)
            _elements[i] *= matrix._elements[i];

        return *this;
    }

    Matrix<Type> &operator=(Matrix<Type> &&matrix) noexcept {

        _row = matrix._row;
        _column = matrix._column;
        _elements = matrix._elements;
        matrix._elements = nullptr;

        return *this;
    }

    Matrix<Type> &operator*=(ssize_t value) {
        for (ssize_t i = 0; i < _row * _column; i++)
            _elements[i] *= value;
        return *this;
    };

    Matrix<Type> &operator/=(ssize_t value) {
        for (ssize_t i = 0; i < _row * _column; i++)
            _elements[i] /= value;
        return *this;
    };

    friend std::ostream &operator<<(std::ostream &stream, const Matrix<Type> &matrix);

private:
    Type *_elements;
    size_t _row;
    size_t _column;
};

template<typename Type>
std::ostream &operator<<(std::ostream &stream, const Matrix<Type> &matrix) {
    stream << "[";
    for (size_t i = 0; i < matrix._row * matrix._column; i++) {
        if (i % matrix._column == 0)
            stream << "]\n[";

        stream << matrix._elements[i];
    }
    stream << "]";

}

#endif