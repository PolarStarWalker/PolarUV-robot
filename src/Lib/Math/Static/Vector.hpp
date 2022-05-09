#ifndef ROBOT_STATICVECTOR_HPP
#define ROBOT_STATICVECTOR_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>

#include "Math/TypeTraits.hpp"

#include <arm_neon.h>
#include <array>

namespace stc {
    enum VectorTypeEnum {
        Horizontal,
        Vertical
    };

    template<VectorTypeEnum VectorType, typename Type, size_t VectorSize> requires std::is_arithmetic_v<Type>
    struct Vector {
    private:

        using Vector_t = Vector<VectorType, Type, VectorSize>;
        using Array_t = std::array<Type, VectorSize>;

        Array_t elements_{};

    public:

        Vector() = default;

        Vector(const Vector_t &vector) = default;

        explicit Vector(const Array_t &array) noexcept;

        Vector(std::initializer_list<Type> list);

        [[nodiscard]] consteval size_t Size() const noexcept { return VectorSize; }

        [[nodiscard]] Type &operator[](size_t index) &{
#ifdef DEBUG
            if (index >= VectorSize)
                throw std::out_of_range("index out of range");
#endif

            return elements_[index];
        }

        [[nodiscard]] const Type &operator[](size_t index) const & {
#ifdef DEBUG
            if (index >= VectorSize)
                throw std::out_of_range("index out of range");
#endif

            return elements_[index];
        }

        Vector_t &operator*=(Type value);

        Vector_t &operator*=(Type value) requires IsFloat32<Type>;

        Vector_t &operator/=(Type value);

        Vector_t &operator+=(Type value);

        Vector_t &operator+=(Type value) requires IsFloat32<Type>;

        [[nodiscard]] Type *begin() noexcept { return elements_.begin(); }

        [[nodiscard]] Type *end() noexcept { return elements_.end(); }

        [[nodiscard]] const Type *cbegin() const noexcept  { return elements_.cbegin(); }

        [[nodiscard]] const Type *cend() const noexcept { return elements_.cend(); }

        [[nodiscard]] auto rbegin() noexcept{ return elements_.rbegin(); }

        [[nodiscard]] auto rend() noexcept { return elements_.rend(); }

        [[nodiscard]] auto rcbegin() const noexcept{ return elements_.crbegin(); }

        [[nodiscard]] auto rcend() const noexcept { return elements_.crend(); }

        void Normalize(Type amplitude);

        void Normalize(Type amplitude) requires IsFloat32<Type>;

        explicit operator std::array<Type, VectorSize>(){
            return elements_;
        }
    };

    template<typename T, size_t VS>
    using HorizontalVector = Vector<Horizontal, T,VS>;

    template<typename T, size_t VS>
    using VerticalVector = Vector<Vertical, T,VS>;

//template<typename TypeArg, typename... Arguments>
//requires (std::is_same_v<TypeArg, Arguments> && ...)
//Vector(VectorTypeEnum VectorType, TypeArg, Arguments...) -> Vector<VectorType, TypeArg, 1 + sizeof...(Arguments)>;

template<VectorTypeEnum VectorType, typename Type, size_t VectorSize> requires std::is_arithmetic_v<Type>
Vector<VectorType, Type, VectorSize>::Vector(const std::initializer_list<Type> list) {
    for (size_t i = 0; i < VectorSize; i++)
        elements_[i] = list.begin()[i];
}

    template<VectorTypeEnum VT, typename T, size_t VS>
    requires std::is_arithmetic_v<T>
    Vector<VT, T, VS>::Vector(const std::array<T, VS> &array) noexcept {
        for (size_t i = 0; i < VS; ++i)
            elements_[i] = array[i];
    }

    template<VectorTypeEnum VT, typename T, size_t VS>
    requires std::is_arithmetic_v<T>
    Vector<VT, T, VS> &Vector<VT, T, VS>::operator*=(T value) {

        for (size_t i = 0; i < VS; ++i) {
            elements_[i] *= value;
        }

        return *this;
    }

    template<VectorTypeEnum VT, typename T, size_t VS> requires std::is_arithmetic_v<T>
    Vector<VT, T, VS> &Vector<VT, T, VS>::operator*=(T value) requires IsFloat32<T> {

        constexpr size_t iterationCount = GetIterationCount<T>(VS);
        constexpr size_t alignment = GetAlignment<T>(VS);

        auto *array = (float32x4_t *) &elements_[0];
        for (size_t i = 0; i < iterationCount; ++i) {
            array[i] = vmulq_n_f32(array[i], value);
        }

        ///If size is 16 byte aligned
        if constexpr (alignment == 0)
            return *this;

        for (size_t i = VS - alignment; i < VS; ++i) {
            elements_[i] *= value;
        }

        return *this;

    }

    template<VectorTypeEnum VT, typename T, size_t VS>
    requires std::is_arithmetic_v<T>
    Vector<VT, T, VS> &Vector<VT, T, VS>::operator/=(T value) {
        for (size_t i = 0; i < VS; ++i) {
            elements_[i] /= value;
        }
    }

    template<VectorTypeEnum VT, typename T, size_t VS>
    requires std::is_arithmetic_v<T>
    Vector<VT, T, VS> &Vector<VT, T, VS>::operator+=(T value) {

        for (size_t i = 0; i < VS; ++i) {
            elements_[i] += value;
        }

        return *this;
    }

    template<VectorTypeEnum VT, typename T, size_t VS>
    requires std::is_arithmetic_v<T>
    Vector<VT, T, VS> & Vector<VT, T, VS>::operator+=(T value) requires IsFloat32<T> {

        constexpr size_t iterationCount = GetIterationCount<T>(VS);
        constexpr size_t alignment = GetAlignment<T>(VS);

        const T valueArray[4] = {value, value, value, value};
        auto &values = (float32x4_t &) valueArray;

        auto *array = (float32x4_t *) &elements_[0];
        for (size_t i = 0; i < iterationCount; ++i) {
            array[i] = vaddq_f32(array[i], values);
        }

        ///If size is 16 byte aligned
        if constexpr (alignment == 0)
            return *this;

        for (size_t i = VS - alignment; i < VS; ++i) {
            elements_[i] += value;
        }

        return *this;

    }

    template<VectorTypeEnum VT,typename T, size_t VS>
    requires std::is_arithmetic_v<T>
    void Vector<VT, T, VS>::Normalize(T amplitude) {
        double maxValue = 0;

        for (size_t i = 0; i < VS; ++i) {
            T absValue = std::abs(elements_[i]);
            if (absValue > maxValue) {
                maxValue = absValue;
            }
        }

        double coefficient = amplitude / maxValue;

        if (coefficient < 1) {
            for (size_t i = 0; i < VS; i++) {
                elements_[i] *= coefficient;
            }
        }
    }

    template<VectorTypeEnum VT,typename Type, size_t VectorSize>
    requires std::is_arithmetic_v<Type>
    void Vector<VT, Type, VectorSize>::Normalize(Type amplitude) requires IsFloat32<Type> {

        constexpr size_t iterationCount = GetIterationCount<Type>(VectorSize);
        constexpr size_t alignment = GetAlignment<Type>(VectorSize);

        double maxValue = 0;

        auto *array = (float32x4_t *) &elements_[0];
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
                Type absValue = std::abs(elements_[i]);
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
            elements_[i] *= coefficient;
        }
    }

    template<typename T, size_t VS> requires std::is_arithmetic_v<T>
    T operator*(const HorizontalVector<T,VS> &l, const VerticalVector<T, VS> &r) requires IsFloat32<T> {

        constexpr size_t iterationCount = GetIterationCount<T>(VS);
        constexpr size_t alignment = GetAlignment<T>(VS);

        const auto *lData = (const float32x4_t *) l.cbegin();
        const auto *rData = (const float32x4_t *) r.cbegin();

        T value = 0.0f;

        for (size_t i = 0; i < iterationCount; ++i) {
            float32x4_t buf = vmulq_f32(lData[i], rData[i]);
            value += vaddvq_f32(buf);
        }

        if constexpr(alignment == 0)
            return value;

        for (size_t i = VS - alignment; i < VS; i++) {
            value += l[i] * r[i];
        }

        return value;
    }

    template<typename Type, size_t VectorSize>
    std::ostream &operator<<(std::ostream &out, const VerticalVector<Type, VectorSize> &vector) {

        out << '[';
        for (ssize_t i = 0; i < VectorSize - 1; i++) {
            out << vector[i] << ", ";
        }
        out << vector[VectorSize - 1];
        out << "]";

        return out;
    }

    template<typename Type, size_t VectorSize>
    std::ostream &operator<<(std::ostream &out, const HorizontalVector<Type, VectorSize> &vector) {

        out << '[';
        for (size_t i = 0; i < VectorSize - 1; i++) {
            out << vector[i] << ", ";
        }
        out << vector[VectorSize - 1];
        out << "]";

        return out;
    }

}
#endif