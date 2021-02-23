#include "FloatMatrixClass.hpp"

FloatMatrixClass::FloatMatrixClass(size_t row, size_t column)
{
	_row = row;
	_column = column;

	_matrix = new double[_row, _column];
}

FloatMatrixClass::~FloatMatrixClass() { delete[] _matrix; }

FloatMatrixClass& FloatMatrixClass::operator=(double* right)
{
	for (size_t i = 0; i < this->GetRows(); i++) {

		for (size_t j = 0; j < this->GetColumns(); j++) {
			
			_matrix[i, j] = right[i, j];
		}
	}
}




