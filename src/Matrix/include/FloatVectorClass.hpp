#ifndef ROV_FLOATVECTORCLASS_HPP
#define ROV_FLOATVECTORCLASS_HPP

#include "BaseVectorClass.hpp"
#include "IntMatrixClass.hpp"
#include "IntVectorClass.hpp"
#include "FloatMatrixClass.hpp"

#include <array>

class FloatMatrixClass;
class IntMatrixClass;
class IntVectorClass;

class FloatVectorClass : public BaseVectorClass {

    friend IntMatrixClass;
    friend IntVectorClass;
    friend FloatMatrixClass;

private:
    double *_vector = nullptr;

    double &operator[](size_t value) const;

    FloatVectorClass(const FloatVectorClass& vector);
    FloatVectorClass(FloatVectorClass&& vector) noexcept;

public:

    explicit FloatVectorClass(size_t length);

    ~FloatVectorClass();

    double &operator[](size_t value);

    FloatVectorClass& operator+=(int64_t value);

    FloatVectorClass &operator=(FloatVectorClass&& right);
    FloatVectorClass &operator=(double *right);
    FloatVectorClass &operator=(float *right);

    void Normalize(double value);
    void FillArray(std::array<uint16_t, 12>* array);

    friend std::ostream &operator<<(std::ostream &stream, const FloatVectorClass &matrixClass);
};

std::ostream &operator<<(std::ostream &stream, const FloatVectorClass &matrixClass);
#endif