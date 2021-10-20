#include "../include/FloatVectorClass.hpp"
#include <iostream>
#include <array>
#include <cmath>

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

FloatVectorClass::FloatVectorClass(FloatVectorClass &&vector) noexcept {
    this->_vector = vector._vector;
    this->_length = vector._length;

    vector._vector = nullptr;
    vector._length = 0;
}

FloatVectorClass::~FloatVectorClass() {
    delete[] this->_vector;
}

double &FloatVectorClass::operator[](size_t value) {
    return this->_vector[value];
}

double &FloatVectorClass::operator[](size_t value) const {
    return this->_vector[value];
}

FloatVectorClass &FloatVectorClass::operator=(double *right) {
    for (size_t i = 0; i < this->_length; i++) {
        this->_vector[i] = right[i];
    }
    return *this;
}


FloatVectorClass &FloatVectorClass::operator=(FloatVectorClass &&right) {
    delete[] this->_vector;

    this->_vector = right._vector;
    this->_length = right._length;
    right._vector = nullptr;

    return *this;
}

FloatVectorClass &FloatVectorClass::operator=(float *right) {
    for (size_t i = 0; i < this->_length; i++) {
        this->_vector[i] = right[i];
    }
    return *this;
}

std::ostream &operator<<(std::ostream &stream, const FloatVectorClass &vectorClass) {
    for (size_t i = 0; i < vectorClass._length; i++) {
        stream << vectorClass._vector[i] << std::endl;
    }
    return stream;
}

FloatVectorClass &FloatVectorClass::operator+=(int64_t value) {
    for (size_t i = 0; i < this->_length; i++) {
        this->_vector[i] += value;
    }
    return *this;
}

void FloatVectorClass::Normalize(double amplitude) {

    double maxValue = 0;

    for (size_t i = 0; i < this->_length; i++) {
        double absValue = std::abs(this->_vector[i]);
        if (absValue > maxValue) {
            maxValue = absValue;
        };
    }

    double coefficient = amplitude / maxValue;

    if (coefficient < 1) {
        for (size_t i = 0; i < this->_length; i++) {
            this->_vector[i] *= coefficient;
        }
    }
}

void FloatVectorClass::FillArray(std::array<uint16_t, 12> *array, size_t offset) const {
    for (size_t i = 0; i < _length; i++) {
        (*array)[i + offset] = std::round(_vector[i]);
    }
}

FloatVectorClass &FloatVectorClass::operator*=(ssize_t value) {
    for(size_t i = 0; i < _length; i++){
        _vector[i] *= value;
    }

    return *this;
}
