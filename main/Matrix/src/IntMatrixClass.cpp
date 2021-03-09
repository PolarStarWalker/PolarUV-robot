#include "../include/IntMatrixClass.hpp"

IntMatrixClass::IntMatrixClass(size_t row, size_t column)
{
	_row = row;
	_column = column;

	_matrix = new int64_t[_row * _column];

}

IntMatrixClass::IntMatrixClass(size_t row)
{
	_row = row;
	_column = 1;

	_matrix = new int64_t[_row];

}


IntMatrixClass::~IntMatrixClass()
{
	delete[] _matrix;
}


int64_t* IntMatrixClass::operator[](size_t value) 
{
	return  &_matrix[value * _column];
}


IntMatrixClass operator*(IntMatrixClass& left, IntMatrixClass& right) {

	if (!left.IfThisMultiplyebleOn(right)) 
	{ 
		IntMatrixClass newIntintMatrixClass(0, 0); 
		return newIntintMatrixClass; 
	}

	IntMatrixClass newIntMatrix(left.GetRows(), right.GetColumns());

	for (size_t m = 0; m < left.GetRows();m++) {
		for (size_t k = 0; k < left.GetColumns(); k++) {
			for (size_t n = 0; n < left.GetColumns(); n++) {
				newIntMatrix[m][k] += left[m][n] * right[n][k];
			}
		}
	}
	return newIntMatrix;
}


IntMatrixClass operator*=(IntMatrixClass& left, FloatMatrixClass& right)
{
	if (!left.IfThisMultiplyebleOn(right))
	{
		IntMatrixClass newIntintMatrixClass(0, 0);
		return newIntintMatrixClass;
	}

	IntMatrixClass newIntMatrix(left.GetRows(), right.GetColumns());

	for (size_t m = 0; m < left.GetRows(); m++) {
		for (size_t k = 0; k < right.GetColumns(); k++) {
			for (size_t n = 0; n < left.GetColumns(); n++) {
				newIntMatrix[m][k] += left[m][n] * right[n][k];
			}
		}
	}
	return newIntMatrix;
}


IntMatrixClass& IntMatrixClass::operator=(int64_t* matrix[])
{

	for (size_t row = 0; row <  this->_row; row++) 
	{
		for (size_t column = 0; column < this->_column; column++) 
		{
			this->operator[](row)[column] = matrix[row][column]; 
		}
	}

	return *this;
}

