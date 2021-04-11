#ifndef ROV_FLOATVECTORCLASS_HPP
#define ROV_FLOATVECTORCLASS_HPP

#include "BaseVectorClass.hpp"

class IntMatrixClass;

class FloatVectorClass : public BaseVectorClass {

private:
    double *_vector = nullptr;

    double &operator[](size_t value) const;

    FloatVectorClass(const FloatVectorClass& vector);

    FloatVectorClass(FloatVectorClass&& vector) noexcept;

public:
    explicit FloatVectorClass(size_t length);

    FloatVectorClass();

    ~FloatVectorClass();

    double &operator[](size_t value);

    FloatVectorClass &operator=(FloatVectorClass&& right);

    FloatVectorClass &operator=(double *right);

    FloatVectorClass &operator=(float *right);

    friend std::ostream &operator<<(std::ostream &stream, const FloatVectorClass &matrixClass);

    friend FloatVectorClass operator*(const IntMatrixClass& matrix, const FloatVectorClass& vector);
};

std::ostream &operator<<(std::ostream &stream, const FloatVectorClass &matrixClass);
#endif