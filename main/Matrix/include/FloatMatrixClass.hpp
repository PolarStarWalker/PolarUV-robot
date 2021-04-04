#pragma once
#include "BaseMatrixClass.hpp"

class FloatMatrixClass : public virtual BaseMatrixClass
{
private:
	double* _matrix;

public:
	FloatMatrixClass(size_t row, size_t column);
    FloatMatrixClass(const FloatMatrixClass &matrix);
	explicit FloatMatrixClass(size_t row);
	~FloatMatrixClass();

	double* operator[](size_t value);
	
	FloatMatrixClass operator*(FloatMatrixClass& right);

	FloatMatrixClass& operator= (double** right);
	FloatMatrixClass& operator= (double* right);
	FloatMatrixClass& operator= (float* right);
};


