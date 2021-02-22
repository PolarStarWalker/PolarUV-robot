#ifndef ROBOT_TYPETRAITS_HPP
#define ROBOT_TYPETRAITS_HPP

#include <type_traits>

template<typename Type>
concept IsFloat32 = std::is_floating_point_v<Type> && (sizeof(Type) == 4);

template<typename Type>
concept IsFloat64 = std::is_floating_point_v<Type> && (sizeof(Type) == 8);

template<typename Type>
concept IsInt8 = std::is_integral_v<Type> && (sizeof(Type) == 1);

template<typename Type>
concept IsInt16 = std::is_integral_v<Type> && (sizeof(Type) == 2);

template<typename Type>
concept IsInt32 = std::is_integral_v<Type> && (sizeof(Type) == 4);

template<typename Type>
concept IsInt64 = std::is_integral_v<Type> && (sizeof(Type) == 8);

template<typename Type>
struct VectorTypeTraits{

    consteval size_t IterationCount(size_t vectorSize) requires IsFloat32<Type> {
        const size_t bytesCount = vectorSize * sizeof(Type);
        return bytesCount / 16;
    }

    consteval size_t Alignment(size_t vectorSize) requires IsFloat32<Type> {
        const size_t bytesCount = vectorSize * sizeof(Type);
        return (bytesCount % 16) / sizeof(Type);
    }

};

template<typename Type>
consteval size_t GetIterationCount(size_t vectorSize) requires IsFloat32<Type> {
    const size_t bytesCount = vectorSize * sizeof(Type);
    return bytesCount / 16;
}

template<typename Type>
consteval size_t GetAlignment(size_t vectorSize) requires IsFloat32<Type> {
    const size_t bytesCount = vectorSize * sizeof(Type);
    return (bytesCount % 16) / sizeof(Type);
}

#endif