#include "../include/IntVectorClass.hpp"

IntVectorClass::IntVectorClass(size_t value){
    this->_length = value;
    this->_matrix = new int64_t[this->_length];
}

IntVectorClass::IntVectorClass(const IntVectorClass& vector) {
    this->_length = vector._length;
    this->_matrix = new int64_t[this->_length];

    for (size_t i = 0; i < this->_length; i++) {
        this->_matrix[i] = vector._matrix[i];
    }
}


IntVectorClass::IntVectorClass(IntVectorClass &&vector) noexcept {
    this->_length = vector._length;
    this->_matrix = vector._matrix;

    vector._matrix = nullptr;
}

IntVectorClass::~IntVectorClass() {
    delete[] this->_matrix;
}

int64_t & IntVectorClass::operator[](size_t value) {
    return *(this->_matrix + value);
}

IntVectorClass &IntVectorClass::operator=(IntVectorClass&& vector) {
    this->_length = vector._length;
    this->_matrix = vector._matrix;

    vector._matrix = nullptr;

    return *this;
}
