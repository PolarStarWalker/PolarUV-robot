#include "../include/FloatVectorClass.hpp"
#include <ostream>

FloatVectorClass::FloatVectorClass(size_t length) {
    this->_vector = new double[length]{};
    this->_length = length;

}

FloatVectorClass::FloatVectorClass(const FloatVectorClass &vector) {
    this->_vector = new double[vector._length];
    this->_length = vector._length;

    for (size_t i = 0; i < this->_length; i++) {
        this->_vector[i] = vector._vector[i];
    }
}

FloatVectorClass::~FloatVectorClass() {
    delete [] this->_vector;
}

double &FloatVectorClass::operator[](size_t value) {
    return this->_vector[value];
}

double &FloatVectorClass::operator[](size_t value) const {
    return this->_vector[value];
}

FloatVectorClass& FloatVectorClass::operator=(double *right) {
    for (size_t i = 0; i < this->_length; i++) {
        this->_vector[i] = right[i];
    }
    return *this;
}

FloatVectorClass& FloatVectorClass::operator=(float *right) {
    for (size_t i = 0; i < this->_length; i++) {
        this->_vector[i] = right[i];
    }
    return *this;
}

std::ostream &operator<<(std::ostream &stream, const FloatVectorClass &vectorClass) {
    for (size_t i = 0; i < vectorClass._length; i++) {
        stream<<vectorClass._vector[i]<<std::endl;
    }
    return stream;
}
