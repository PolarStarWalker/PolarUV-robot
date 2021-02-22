#include "IntMatrixClass.hpp"


IntMatrixClass::IntMatrixClass(size_t row, size_t column)
{
	*_row = row;
	*_column = column;

	_matrix = new int64_t[*_row, *_column];
}

IntMatrixClass::~IntMatrixClass()
{
	delete[] _matrix;
	delete _row;
	delete _column;
}


int64_t& IntMatrixClass::operator[](size_t value) 
{
	return _matrix[value];
}


IntMatrixClass operator*(IntMatrixClass& left, IntMatrixClass& right) {

	if (left.GetColumns() != right.GetRows()) { return IntMatrixClass(0,0); }
	

}


size_t IntMatrixClass::GetRows() { return *_row; }
size_t IntMatrixClass::GetColumns() { return *_column; }
