#pragma once
#include <BaseMatrixClass.hpp>


class IntMatrixClass : virtual BaseMatrixClass
{
	int64_t** _matrix;
	
public:

	IntMatrixClass(size_t row, size_t column);
	~IntMatrixClass();

	int64_t* operator[](size_t value);

	IntMatrixClass& operator*(BaseMatrixClass& right);
	IntMatrixClass& operator=(int64_t* right);

};

