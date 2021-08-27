#ifndef ROV_FLOATMATRIXCLASS_HPP
#define ROV_FLOATMATRIXCLASS_HPP

#include "BaseMatrixClass.hpp"
#include "FloatVectorClass.hpp"
#include "IntVectorClass.hpp"

class IntMatrixClass;

class FloatVectorClass;

class IntVectorClass;

class FloatMatrixClass : public virtual BaseMatrixClass {
    friend IntMatrixClass;
    friend FloatVectorClass;
    friend IntVectorClass;

private:
    double *_matrix = nullptr;

    double *operator[](size_t value) const;

    FloatMatrixClass(const FloatMatrixClass &matrix);

    FloatMatrixClass(FloatMatrixClass &&matrix) noexcept;

public:
    FloatMatrixClass(size_t row, size_t column);

    ~FloatMatrixClass();

    double *operator[](size_t value);

    FloatMatrixClass operator*(FloatMatrixClass &right);

    FloatMatrixClass &operator*=(int64_t value);

    FloatVectorClass operator*(const FloatVectorClass &vector);

    FloatMatrixClass &operator=(double **right);

    FloatMatrixClass &operator=(const double *right);

    FloatMatrixClass &operator=(float *right);

    FloatMatrixClass &operator=(FloatMatrixClass &&floatMatrix);

    friend std::ostream &operator<<(std::ostream &stream, const FloatMatrixClass &matrixClass);
};

std::ostream &operator<<(std::ostream &stream, const FloatMatrixClass &matrixClass);

#endif