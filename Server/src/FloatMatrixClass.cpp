#include "FloatMatrixClass.hpp"

FloatMatrixClass::FloatMatrixClass(size_t row, size_t column)
{
	_row = row;
	_column = column;

	_matrix = new double*[_row] {};

	for (size_t i = 0; i < _row; i++) { _matrix[i] = new double[_column]; }
}

FloatMatrixClass::~FloatMatrixClass() {
	for (size_t i = 0; i < _row; i++) { delete _matrix[i]; }
	delete[] _matrix;
}

double* FloatMatrixClass::operator[](size_t value) { return  _matrix[value]; }


/*
FloatMatrixClass& FloatMatrixClass::operator*(FloatMatrixClass& right)
{
	// TODO: вставьте здесь оператор return
}

FloatMatrixClass& FloatMatrixClass::operator*(IntMatrixClass& right)
{
	// TODO: вставьте здесь оператор return
}

*/

FloatMatrixClass& FloatMatrixClass::operator=(double** right)
{
	for (size_t i = 0; i < this->GetRows(); i++) {

		for (size_t j = 0; j < this->GetColumns(); j++) {
			
			_matrix[i][j] = right[i][j];
		}
	}

	return *this;
}




