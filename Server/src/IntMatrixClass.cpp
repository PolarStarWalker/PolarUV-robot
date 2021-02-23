#include "IntMatrixClass.hpp"
#include <iostream>

IntMatrixClass::IntMatrixClass(size_t row, size_t column)
{
	_row = row;
	_column = column;

	_matrix = new int64_t*[_row];

	for (size_t i = 0; i < _row; i++) { _matrix[i] = new int64_t[_column] {}; }
}

IntMatrixClass::~IntMatrixClass()
{
	delete[] _matrix;
}


int64_t* IntMatrixClass::operator[](size_t value) 
{
	return  _matrix[value];
}


IntMatrixClass& IntMatrixClass::operator*(IntMatrixClass& right) {

	if (!this->IfThisMultiplyebleOn(right)) 
	{ 
		IntMatrixClass newIntintMatrixClass(0, 0); 
		return newIntintMatrixClass; 
	}

	IntMatrixClass newIntMatrix(this->GetRows(), right.GetColumns());

	for (size_t m = 0; m < this->GetRows();m++) {
		for (size_t k = 0; k < right.GetColumns(); k++) {
			for (size_t n = 0; n < this->GetColumns(); n++) {
				newIntMatrix[m][k] += this->_matrix[m][n] * right[n][k];
			}
		}
	}
	return newIntMatrix;
}

/*
IntMatrixClass& IntMatrixClass::operator*(FloatMatrixClass& right)
{
	if (!this->IfThisMultiplyebleOn(right))
	{
		IntMatrixClass newIntintMatrixClass(0, 0);
		return newIntintMatrixClass;
	}

	IntMatrixClass newIntMatrix(this->GetRows(), right.GetColumns());

	for (size_t m = 0; m < this->GetRows(); m++) {
		for (size_t k = 0; k < right.GetColumns(); k++) {
			for (size_t n = 0; n < this->GetColumns(); n++) {
				newIntMatrix[m][k] += this->_matrix[m][n] * right[n][k];
			}
		}
	}
	return newIntMatrix;
}
*/

IntMatrixClass& IntMatrixClass::operator=(int64_t* matrix[])
{

	for (size_t m = 0; m <  this->_row; m++) 
	{
		for (size_t n = 0; n < this->_column; n++) 
		{
			this->_matrix[m][n] = matrix[m][n]; 
		}
	}

	return *this;
}

