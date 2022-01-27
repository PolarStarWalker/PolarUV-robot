#ifndef ROBOT_STATICVECTOR_HPP
#define ROBOT_STATICVECTOR_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>

#include "../TypeTraits.hpp"

#include <arm_neon.h>
#include <array>

template<typename Type, size_t VectorSize> requires std::is_arithmetic_v<Type>
class StaticVector {
private:
    Type _elements[VectorSize];

    using StaticVectorType = StaticVector<Type, VectorSize>;

public:

    StaticVector() : _elements{} {};

    StaticVector(const StaticVector<Type, VectorSize> &vector) noexcept;

    explicit StaticVector(const std::array<Type, VectorSize> &array) noexcept;

    StaticVector(std::initializer_list<Type> list);

    explicit StaticVector(const Type *data, size_t size) noexcept;

    ~StaticVector() = default;

    [[nodiscard]] size_t Size() const noexcept { return VectorSize; }

    Type &operator[](size_t index) { return _elements[index]; }

    const Type &operator[](size_t index) const { return _elements[index]; }

    StaticVectorType &operator*=(Type value);

    StaticVectorType &operator*=(Type value) requires IsFloat32<Type>;

    StaticVectorType &operator/=(Type value);

    StaticVectorType &operator+=(Type value);

    StaticVectorType &operator+=(Type value) requires IsFloat32<Type>;

    Type *begin() { return _elements; }

    Type *end() { return _elements + VectorSize; }

    void Normalize(Type amplitude);

    void Normalize(Type amplitude) requires IsFloat32<Type>;

    friend std::ostream &operator<<(std::ostream &stream, const StaticVector<Type, VectorSize> &vector) {

        stream << '[';
        for (ssize_t i = 0; i < VectorSize - 1; i++) {
            stream << vector[i] << ", ";
        }
        stream << vector[VectorSize - 1];
        stream << "]\n";

        return stream;
    };

};

template<typename TypeArg, typename... Arguments>
StaticVector(TypeArg, Arguments... ) ->
StaticVector<std::enable_if_t<(std::is_same_v<TypeArg, Arguments> && ...), TypeArg>, 1 + sizeof...(Arguments)>;

template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
StaticVector<Type, VectorSize>::StaticVector(const std::initializer_list<Type> list) {
    for (size_t i = 0; i < VectorSize; i++)
        _elements[i] = list.begin()[i];
}

template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
StaticVector<Type, VectorSize>::StaticVector(const StaticVector<Type, VectorSize> &vector) noexcept {
    for (size_t i = 0; i < VectorSize; i++)
        _elements[i] = vector._elements[i];
}

template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
StaticVector<Type, VectorSize>::StaticVector(const std::array<Type, VectorSize> &array) noexcept {
    for (size_t i = 0; i < VectorSize; ++i)
        _elements[i] = array[i];
}


template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
StaticVector<Type, VectorSize>::StaticVector(const Type *data, size_t Size) noexcept {
    for (size_t i = 0; i < Size; ++i)
        _elements[i] = data[i];
}

template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
StaticVector<Type, VectorSize> &StaticVector<Type, VectorSize>::operator*=(Type value) {

    for (size_t i = 0; i < VectorSize; ++i) {
        _elements[i] *= value;
    }

    return *this;
}

template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
StaticVector<Type, VectorSize> &StaticVector<Type, VectorSize>::operator*=(Type value) requires IsFloat32<Type> {

    constexpr size_t iterationCount = GetIterationCount<Type>(VectorSize);
    constexpr size_t alignment = GetAlignment<Type>(VectorSize);

    auto *array = (float32x4_t *) _elements;
    for (size_t i = 0; i < iterationCount; ++i) {
        array[i] = vmulq_n_f32(array[i], value);
    }

    ///If size is 16 byte aligned
    if constexpr (alignment == 0)
        return *this;

    for (size_t i = VectorSize - alignment; i < VectorSize; ++i) {
        _elements[i] *= value;
    }

    return *this;

}

template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
StaticVector<Type, VectorSize> &StaticVector<Type, VectorSize>::operator/=(Type value) {
    for (size_t i = 0; i < VectorSize; ++i) {
        _elements[i] /= value;
    }
}

template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
StaticVector<Type, VectorSize> &StaticVector<Type, VectorSize>::operator+=(Type value) {

    for (size_t i = 0; i < VectorSize; ++i) {
        _elements[i] += value;
    }

    return *this;
}

template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
StaticVector<Type, VectorSize> &StaticVector<Type, VectorSize>::operator+=(Type value) requires IsFloat32<Type> {

    constexpr size_t iterationCount = GetIterationCount<Type>(VectorSize);
    constexpr size_t alignment = GetAlignment<Type>(VectorSize);

    const Type valueArray[4] = {value, value, value, value};
    auto &values = (float32x4_t &) valueArray;

    auto *array = (float32x4_t *) _elements;
    for (size_t i = 0; i < iterationCount; ++i) {
        array[i] = vaddq_f32(array[i], values);
    }

    ///If size is 16 byte aligned
    if constexpr (alignment == 0)
        return *this;

    for (size_t i = VectorSize - alignment; i < VectorSize; ++i) {
        _elements[i] += value;
    }

    return *this;

}

template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
void StaticVector<Type, VectorSize>::Normalize(Type amplitude) {
    double maxValue = 0;

    for (size_t i = 0; i < VectorSize; ++i) {
        Type absValue = std::abs(_elements[i]);
        if (absValue > maxValue) {
            maxValue = absValue;
        }
    }

    double coefficient = amplitude / maxValue;

    if (coefficient < 1) {
        for (size_t i = 0; i < VectorSize; i++) {
            _elements[i] *= coefficient;
        }
    }
}

template<typename Type, size_t VectorSize>
requires std::is_arithmetic_v<Type>
void StaticVector<Type, VectorSize>::Normalize(Type amplitude) requires IsFloat32<Type> {

    constexpr size_t iterationCount = GetIterationCount<Type>(VectorSize);
    constexpr size_t alignment = GetAlignment<Type>(VectorSize);

    double maxValue = 0;

    auto *array = (float32x4_t *) _elements;
    for (size_t i = 0; i < iterationCount; ++i) {
        ///find ABS
        float32x4_t abs = vabsq_f32(array[i]);

        ///find MAX
        float value = vmaxnmvq_f32(abs);

        if (value > maxValue)
            maxValue = value;
    }

    ///If size is not 16 byte aligned
    if (alignment != 0) {
        for (size_t i = VectorSize - alignment; i < VectorSize; ++i) {
            Type absValue = std::abs(_elements[i]);
            if (absValue > maxValue) {
                maxValue = absValue;
            }
        }
    }

    float coefficient = amplitude / maxValue;

    if (coefficient >= 1)
        return;

    for (size_t i = 0; i < iterationCount; i++)
        array[i] = vmulq_n_f32(array[i], coefficient);

    ///If size is 16 byte aligned
    if (alignment == 0)
        return;

    for (size_t i = VectorSize - alignment; i < VectorSize; ++i) {
        _elements[i] *= coefficient;
    }
}




#endif