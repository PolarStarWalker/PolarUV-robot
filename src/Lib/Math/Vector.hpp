#ifndef ROBOT_VECTOR_HPP
#define ROBOT_VECTOR_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>

#include <array>

template<typename Type> requires(std::is_arithmetic_v<Type>)
class Matrix;

template<typename Type> requires(std::is_arithmetic_v<Type>)
class Vector {

    friend Matrix<Type>;

private:
    Type *_elements;
    const size_t _size;

public:
    explicit Vector(size_t size) noexcept : _size(size)  {
        _elements = new Type[_size]{};
    }

    Vector(const Vector<Type> &vector) noexcept : _size(vector._size)  {
        _elements = new Type[_size]{};
        for (size_t i = 0; i < _size; i++) {
            _elements[i] = vector._elements[i];
        }
    }

    Vector(Vector<Type> &&vector) noexcept : _size(vector._size)  {
        _elements = vector._elements;
        vector._elements = nullptr;
    }


    Vector(std::initializer_list<Type> initializerList) :_size(initializerList.size())
    {
        _elements = new Type[_size]{};
        for (size_t i = 0; i < _size; i++) {
            _elements[i] = begin(initializerList)[i];
        }
    }

    ~Vector() { delete[] _elements; }

    inline size_t Size() const { return _size; }

    Type &operator[](size_t index) { return _elements[index]; }

    const Type &operator[](size_t index) const { return _elements[index]; }

    Vector<Type> &operator=(const Type *array) {
        for (size_t i = 0; i < _size; i++) {
            _elements[i] = array[i];
        }
        return *this;
    }

    Vector<Type> &operator*=(Type value){
        for (size_t i = 0; i < _size; ++i) {
            _elements[i] *= value;
        }
        return *this;
    }

    Vector<Type> &operator/=(Type value){
        for (size_t i = 0; i < _size; ++i) {
            _elements[i] /= value;
        }
    }

    Vector<Type> &operator+=(Type value){
        for (size_t i = 0; i < _size; ++i) {
            _elements[i] += value;
        }

        return *this;
    }

    void Normalize(Type amplitude){
        double maxValue = 0;

        for (size_t i = 0; i < _size; i++) {
            Type absValue = std::abs(_elements[i]);
            if (absValue > maxValue) {
                maxValue = absValue;
            };
        }

        double coefficient = amplitude / maxValue;

        if (coefficient < 1) {
            for (size_t i = 0; i < _size; i++) {
                _elements[i] *= coefficient;
            }
        }
    }

    void FillArray(std::array<uint16_t, 12> *array, size_t offset = 0) const{
        for(size_t i = 0; i< _size; ++i){
            (*array)[i + offset] = _elements[i];
        }
    }

    friend std::ostream &operator<<(std::ostream &stream, const Vector<Type> &vector) {

        stream << '[';
        for (ssize_t i = 0; i < vector._size - 1; i++) {
            stream << vector[i] << ", ";
        }
        stream << vector[vector._size - 1];
        stream << "]\n";

        return stream;
    };

};



#endif