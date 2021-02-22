#pragma once

#include <stdint.h>
#include <stddef.h>



class IntMatrixClass
{

public:

	int64_t* _matrix;

	size_t* _row = 0;
	size_t* _column = 0;

	IntMatrixClass(size_t row, size_t column);

	~IntMatrixClass();

	int64_t& operator[](size_t value);

	friend IntMatrixClass operator*(IntMatrixClass& left, IntMatrixClass& right);

private:

	size_t GetColumns();
	size_t GetRows();
};

IntMatrixClass operator*(IntMatrixClass& left, IntMatrixClass& right);