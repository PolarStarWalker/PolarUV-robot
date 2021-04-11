#ifndef ROV_FLOATMATRIXCLASS_HPP
#define ROV_FLOATMATRIXCLASS_HPP

#include "BaseMatrixClass.hpp"

class FloatMatrixClass : public virtual BaseMatrixClass
{
private:
	double* _matrix =nullptr;

    double* operator[](size_t value)const;

    FloatMatrixClass(const FloatMatrixClass &matrix);

    FloatMatrixClass(FloatMatrixClass&& matrix) noexcept ;

public:
	FloatMatrixClass(size_t row, size_t column);

	~FloatMatrixClass();

	double* operator[](size_t value);
	
	FloatMatrixClass operator*(FloatMatrixClass& right);

	FloatMatrixClass& operator= (double** right);
	FloatMatrixClass& operator= (double* right);
	FloatMatrixClass& operator= (float* right);

    friend std::ostream& operator<<(std::ostream& stream, const FloatMatrixClass& matrixClass);
};

std::ostream& operator<<(std::ostream& stream, const FloatMatrixClass& matrixClass);

#endif