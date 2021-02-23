#pragma once
#include <stdint.h>
#include <stddef.h>

class BaseMatrixClass
{
protected:

	size_t _row = 0;
	size_t _column = 0;


public:

	size_t GetRows() { return _row; }
	size_t GetColumns() { return _column; }

	virtual ~BaseMatrixClass() = 0;

	virtual BaseMatrixClass& operator(void* right) = 0;

	virtual BaseMatrixClass& operator*(BaseMatrixClass& right) = 0;
};
