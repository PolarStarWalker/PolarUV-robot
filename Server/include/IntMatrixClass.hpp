#pragma once
#include "BaseMatrixClass.hpp"
#include "FloatMatrixClass.hpp"

class IntMatrixClass : public virtual BaseMatrixClass
{
private:
	int64_t* _matrix;
	
public:
	IntMatrixClass(size_t row, size_t column);
	~IntMatrixClass();

	int64_t* operator[](size_t value);

	friend IntMatrixClass& operator*(IntMatrixClass& left, IntMatrixClass& right);

	IntMatrixClass& operator=(int64_t* matrix[]);

};

IntMatrixClass& operator*(IntMatrixClass& left, IntMatrixClass& right);

