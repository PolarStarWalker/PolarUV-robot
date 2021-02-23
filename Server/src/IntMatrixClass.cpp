#include "IntMatrixClass.hpp"


IntMatrixClass::IntMatrixClass(size_t row, size_t column)
{
	_row = row;
	_column = column;

	_matrix = new int64_t[_row, _column];
}

IntMatrixClass::~IntMatrixClass()
{
	delete[] _matrix;
}


int64_t* IntMatrixClass::operator[](size_t value) 
{
	return (int64_t*) _matrix;
}


IntMatrixClass& IntMatrixClass::operator*(BaseMatrixClass& right) {

	if (this->GetColumns() != right.GetRows()) 
	{ 
		IntMatrixClass newIntintMatrixClass(0, 0); 
		return newIntintMatrixClass; 
	}

	IntMatrixClass newIntMatrixClass(this->GetRows(), right.GetColumns());

	for (size_t i = 0; i < this->GetRows();i++) {
		for (size_t j = 0; j < right.GetColumns(); j++) {
			
			int64_t value = 0;

			for (size_t k = 0; k < this->GetColumns(); k++) {
				value+=	this->_matrix[i][k] * right[k][j]
			
			}
		
		}
	
	}


}

IntMatrixClass& IntMatrixClass::operator=(int64_t* right)
{
	// TODO: вставьте здесь оператор return
}

