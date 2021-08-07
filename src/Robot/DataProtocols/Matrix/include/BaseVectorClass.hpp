#pragma once
#include <cstdint>
#include <cstddef>

class BaseVectorClass{
protected:

    mutable size_t _length = 0;

public:

    inline bool IfAddOrSub(BaseVectorClass& vector) const
    {
        return this->_length == vector._length;
    }

    inline size_t Length() const { return _length; }
};
