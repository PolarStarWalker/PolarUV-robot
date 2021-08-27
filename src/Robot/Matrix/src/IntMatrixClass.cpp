#include <cstring>
#include "../include/IntMatrixClass.hpp"
#include "../include/FloatVectorClass.hpp"

#include <cmath>
#include <iostream>

IntMatrixClass::IntMatrixClass(size_t row, size_t column) {
    _row = row;
    _column = column;

    _matrix = new int64_t[_row * _column]{};

}

IntMatrixClass::IntMatrixClass(const IntMatrixClass& matrix) {

    this->_column = matrix._column;
    this->_row = matrix._row;
    this->_matrix = new int64_t[this->_row * this->_row];

    for (size_t i = 0; i <= this->_column * this->_row; i++) {
        this->_matrix[i] = matrix._matrix[i];
    }
}

IntMatrixClass::IntMatrixClass(IntMatrixClass&& matrix) noexcept {
    this->_row = matrix._row;
    this->_column = matrix._column;
    this->_matrix = matrix._matrix;

    matrix._matrix = nullptr;
}


IntMatrixClass::~IntMatrixClass() {
    delete[] _matrix;
}


int64_t *IntMatrixClass::operator[](size_t value) {
    return &_matrix[value * _column];
}

int64_t *IntMatrixClass::operator[](size_t value) const {
    return &_matrix[value * _column];
}

IntMatrixClass IntMatrixClass::operator*(IntMatrixClass &intMatrix) {

    if (!this->IfThisMultiplyebleOn(intMatrix)) {
        IntMatrixClass newIntMatrixClass(0, 0);
        return newIntMatrixClass;
    }

    IntMatrixClass newIntMatrix(this->_row, intMatrix._column);

    for (size_t m = 0; m < this->_row; m++) {
        for (size_t k = 0; k < this->_column; k++) {
            for (size_t n = 0; n < this->_column; n++) {
                newIntMatrix[m][k] += this->operator[](m)[n] * intMatrix[n][k];
            }
        }
    }

    return newIntMatrix;

}


IntMatrixClass IntMatrixClass::operator*(const FloatMatrixClass &matrix) {
    if (!this->IfThisMultiplyebleOn(matrix)) {
        IntMatrixClass newIntMatrixClass(0, 0);
        return newIntMatrixClass;
    }

    IntMatrixClass newIntMatrix(this->_row, matrix._column);

    for (size_t m = 0; m < this->_row; m++) {
        for (size_t k = 0; k < this->_column; k++) {
            for (size_t n = 0; n < this->_column; n++) {
                newIntMatrix[m][k] += this->operator[](m)[n] * matrix[n][k];
            }
        }
    }
    return newIntMatrix;
}


IntMatrixClass& IntMatrixClass::operator=(int64_t *matrix[]) {

    for (size_t row = 0; row < this->_row; row++) {
        for (size_t column = 0; column < this->_column; column++) {
            this->operator[](row)[column] = matrix[row][column];
        }
    }

    return *this;
}

IntMatrixClass& IntMatrixClass::operator=(int64_t *matrix) {
    for (size_t i = 0; i <= this->_column * this->_row; i++) {
        this->_matrix[i] = matrix[i];
    }

    return *this;
}

std::ostream &operator<<(std::ostream &stream, const IntMatrixClass &matrixClass) {
    for (size_t row = 0; row < matrixClass.GetRows(); row++) {
        for (size_t column = 0; column < matrixClass.GetColumns(); column++) {
            stream << (matrixClass.operator[](row)[column]) << ' ';
        }
        stream << std::endl;
    }
    return stream;
}

IntVectorClass IntMatrixClass::operator*(const FloatVectorClass &vector) {
    if (this->_column != vector.Length()) {
        return IntVectorClass(0);
    }

    IntVectorClass vectorClass(this->_row);

    for (size_t m = 0; m < this->_row; m++) {
        for (size_t n = 0; n < vector._length; n++) {
            vectorClass[m] += this->operator[](m)[n] * std::round(vector[n]);
        }
    }
    return vectorClass;
}

IntMatrixClass& IntMatrixClass::operator=(const FloatMatrixClass& floatMatrix) {
    for(size_t i = 0; i< this->_row * this->_column; i++){
        this->_matrix[i] = this->_matrix[i] * std::round(floatMatrix._matrix[i]);
    }

    return *this;
}

IntMatrixClass& IntMatrixClass::operator=(IntMatrixClass&& intMatrix) {
    delete[] this->_matrix;

    this->_matrix = intMatrix._matrix;
    this->_row = intMatrix._row;
    this->_column = intMatrix._column;

    intMatrix._matrix = nullptr;

    return *this;
}

IntMatrixClass IntMatrixClass::operator*(int64_t value) {
    for(size_t i = 0; i< this->_row * this->_column; i++){
        this->_matrix[i] = this->_matrix[i] * value;
    }

    return  *this;
}


