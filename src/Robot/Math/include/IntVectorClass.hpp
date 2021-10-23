//
// Created by Eva on 15.04.2021.
//

#ifndef ROV_INTVECTORCLASS_HPP
#define ROV_INTVECTORCLASS_HPP

#include "BaseVectorClass.hpp"
#include "IntMatrixClass.hpp"
#include "FloatMatrixClass.hpp"
#include "FloatVectorClass.hpp"

class IntMatrixClass;
class FloatMatrixClass;
class FloatVectorClass;

class IntVectorClass : BaseVectorClass{
    friend IntMatrixClass;
    friend FloatMatrixClass;
    friend FloatVectorClass;
private:

    int64_t* _matrix = nullptr;

    IntVectorClass(const IntVectorClass& vector);
    IntVectorClass(IntVectorClass&& vector) noexcept;

public:

    explicit IntVectorClass(size_t length);
    ~IntVectorClass();

    int64_t& operator[](size_t value);

    IntVectorClass& operator=(IntVectorClass&& intVectorClass);

};

#endif //ROV_INTVECTORCLASS_HPP
