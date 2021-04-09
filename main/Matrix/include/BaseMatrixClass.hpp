#pragma once
#include <cstdint>
#include <cstddef>

class BaseMatrixClass
{
protected:
	mutable size_t _row = 0;
    mutable size_t _column = 0;

public:

	inline bool IfThisMultiplyebleOn (BaseMatrixClass& matrix) const
	{
		return this->_column == matrix._row;
	}

	inline bool IfAddOrSub(BaseMatrixClass& matrix) const
	{
		return this->_column == matrix._column && this->_row == matrix._row;
	}

    inline size_t GetRows() const { return _row; }
    inline size_t GetColumns() const { return _column; }
};
