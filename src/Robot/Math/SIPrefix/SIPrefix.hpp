#ifndef ROBOT_SIPREFIX_HPP
#define ROBOT_SIPREFIX_HPP
#include <cstdint>
#include <cstddef>
#include <type_traits>

template<typename Type>
requires(std::is_arithmetic_v<Type>)
constexpr inline Type Nano(Type number){
    return number / 1000;
}

template<typename Type>
requires(std::is_arithmetic_v<Type>)
constexpr inline Type Micro(Type number){
    return number / 1000000;
}

template<typename Type>
requires(std::is_arithmetic_v<Type>)
constexpr inline Type Kilo(Type number){
    return number * 1000;
}

template<typename Type>
requires(std::is_arithmetic_v<Type>)
constexpr inline Type Mega(Type number){
    return number * 1000000;
}

template<typename Type>
requires(std::is_arithmetic_v<Type>)
    constexpr inline Type Giga(Type number){
return number * 1000000000;
}

template<typename Type>
requires(std::is_arithmetic_v<Type>)
constexpr inline Type Tera(Type number){
    return number * 1000000000000;
}

#endif