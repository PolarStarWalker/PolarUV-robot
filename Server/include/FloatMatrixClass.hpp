#pragma once
#include "BaseMatrixClass.hpp"


class FloatMatrixClass : virtual public BaseMatrixClass
{
private:
	double** _matrix;

public:
	FloatMatrixClass(size_t row, size_t column);
	~FloatMatrixClass();

	FloatMatrixClass& operator= (double* right);

	double* operator[](size_t value);

	FloatMatrixClass& operator*(BaseMatrixClass& right);

};

