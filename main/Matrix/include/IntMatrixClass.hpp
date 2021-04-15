#ifndef ROV_INTMATRIXCLASS_HPP
#define ROV_INTMATRIXCLASS_HPP

#include <ostream>
#include "BaseMatrixClass.hpp"
#include "FloatMatrixClass.hpp"
#include "FloatVectorClass.hpp"

class IntMatrixClass : public virtual BaseMatrixClass {
private:
    int64_t *_matrix = nullptr;

    int64_t *operator[](size_t value)const;

    IntMatrixClass(const IntMatrixClass& matrix);

    IntMatrixClass(IntMatrixClass&& matrix) noexcept ;

public:
    IntMatrixClass(size_t row, size_t column);

    ~IntMatrixClass();

    int64_t *operator[](size_t value);

    IntMatrixClass operator*(int64_t value);
    IntMatrixClass operator*(IntMatrixClass &intMatrix);
    IntMatrixClass operator*(const FloatMatrixClass &right);

    IntVectorClass operator*(const FloatVectorClass& vector);

    IntMatrixClass& operator=(int64_t *matrix[]);
    IntMatrixClass& operator=(int64_t matrix[]);
    IntMatrixClass& operator=(IntMatrixClass&& intMatrix);
    IntMatrixClass& operator=(const FloatMatrixClass& floatMatrix);

    friend std::ostream& operator<<(std::ostream& stream, const IntMatrixClass& matrixClass);
};

IntMatrixClass operator*(IntMatrixClass &left, FloatMatrixClass &right);

std::ostream& operator<<(std::ostream& stream, const IntMatrixClass& matrixClass);

#endif