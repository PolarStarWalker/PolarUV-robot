#pragma once

#include <ostream>
#include "BaseMatrixClass.hpp"
#include "FloatMatrixClass.hpp"

class IntMatrixClass : public virtual BaseMatrixClass {
private:
    int64_t *_matrix = nullptr;

    int64_t *operator[](size_t value)const;
public:
    IntMatrixClass(size_t row, size_t column);

    IntMatrixClass(const IntMatrixClass &matrix);

    explicit IntMatrixClass(size_t row);

    ~IntMatrixClass();

    int64_t *operator[](size_t value);


    friend IntMatrixClass operator*(IntMatrixClass &left, IntMatrixClass &right);

    friend IntMatrixClass operator*(IntMatrixClass &left, FloatMatrixClass &right);


    IntMatrixClass &operator=(int64_t *matrix[]);

    IntMatrixClass &operator=(int64_t matrix[]);

    friend std::ostream& operator<<(std::ostream& stream, const IntMatrixClass& matrixClass);
};

IntMatrixClass operator*(IntMatrixClass &left, IntMatrixClass &right);

IntMatrixClass operator*(IntMatrixClass &left, FloatMatrixClass &right);

std::ostream& operator<<(std::ostream& stream, const IntMatrixClass& matrixClass);