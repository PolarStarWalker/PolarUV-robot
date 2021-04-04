#pragma once
#include <stdint.h>
#include <stddef.h>

class BaseMatrixClass
{
protected:
	size_t _row = 0;
	size_t _column = 0;

public:
	inline bool IfThisMultiplyebleOn(BaseMatrixClass& matrix) 
	{
		return this->_column == matrix._row;
	}

	inline bool IfAddOrSub(BaseMatrixClass& matrix) 
	{
		return this->_column == matrix._column && this->_row == matrix._row;
	}

	size_t GetRows() { return _row; }
	size_t GetColumns() { return _column; }
};
