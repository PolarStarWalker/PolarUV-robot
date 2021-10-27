#ifndef ROBOT_STATICVECTOR_HPP
#define ROBOT_STATICVECTOR_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <ostream>

#include <arm_neon.h>

template<typename Type, size_t VectorSize> requires(std::is_arithmetic_v<Type>)
class StaticVector {
private:
    ///Note: array with 16 bytes alligment
    Type _elements[VectorSize]{};

public:

    StaticVector() = default;

    explicit StaticVector(const Type array[VectorSize]) noexcept {
        for (size_t i = 0; i < VectorSize; i++)
            _elements[i] = array[i];
    }

    StaticVector(const StaticVector<Type, VectorSize> &vector) noexcept {
        for (size_t i = 0; i < VectorSize; i++)
            _elements[i] = vector._elements[i];

    }

    StaticVector(const Type *array, size_t rows) noexcept {
        for (size_t i = 0; i < rows; ++i)
            _elements[i] = array[i];
    }

    ~StaticVector() = default;

    size_t Size() const noexcept { return VectorSize; }

    Type &operator[](size_t index) { return _elements[index]; }

    const Type &operator[](size_t index) const { return _elements[index]; }

    StaticVector<Type, VectorSize> &operator=(const Type array[VectorSize]) {
        for (size_t i = 0; i < VectorSize; i++) {
            _elements[i] = array[i];
        }
        return *this;
    }

    StaticVector<Type, VectorSize> &operator*=(Type value) {

        const size_t bytesCount = VectorSize * sizeof(Type);
        const size_t interationCount = bytesCount / 16;
        const size_t aligment = (bytesCount % 16) / sizeof(Type);

        ///If using float32_t
        if (std::is_floating_point_v<Type> && sizeof(Type) == 4) {

            for (size_t i = 0; i < interationCount; ++i) {
                *((float32x4_t *) &(_elements[i * sizeof(Type)])) =
                        vmulq_n_f32(*((float32x4_t *) &(_elements[i * sizeof(Type)])), value);
            }

            ///If size is 16 byte aligned
            if (aligment == 0)
                return *this;

            for (size_t i = VectorSize - aligment; i < VectorSize; ++i) {
                _elements[i] *= value;
            }

            return *this;
        }


        for (size_t i = 0; i < VectorSize; ++i) {
            _elements[i] *= value;
        }
        return *this;
    }

    StaticVector<Type, VectorSize> &operator/=(Type value) {
        for (size_t i = 0; i < VectorSize; ++i) {
            _elements[i] /= value;
        }
    }

    StaticVector<Type, VectorSize> &operator+=(Type value) {

        const size_t bytesCount = VectorSize * sizeof(Type);
        const size_t interationCount = bytesCount / 16;
        const size_t aligment = (bytesCount % 16) / sizeof(Type);

        ///If using float32_t
        if (std::is_floating_point_v<Type> && sizeof(Type) == 4) {

            float values[4] = {value, value, value, value};

            for (size_t i = 0; i < interationCount; ++i) {
                *((float32x4_t *) &(_elements[i * sizeof(Type)])) =
                        vaddq_f32(*((float32x4_t *) &(_elements[i * sizeof(Type)])), *((float32x4_t *) values));
            }

            ///If size is 16 byte aligned
            if (aligment == 0)
                return *this;

            for (size_t i = VectorSize - aligment; i < VectorSize; ++i) {
                _elements[i] += value;
            }

            return *this;
        }

        ///other Types

        for (size_t i = 0; i < VectorSize; ++i) {
            _elements[i] += value;
        }

        return *this;
    }

    void Normalize(Type amplitude) {

        const size_t bytesCount = VectorSize * sizeof(Type);
        const size_t interationCount = bytesCount / 16;
        const size_t aligment = (bytesCount % 16) / sizeof(Type);

        ///If using float32_t
        if (std::is_floating_point_v<Type> && sizeof(Type) == 4) {

            double maxValue = 0;

            for (size_t i = 0; i < interationCount; ++i) {
                ///find ABS
                float32x4_t abs = vabsq_f32(*((float32x4_t *) &(_elements[i * sizeof(Type)])));

                ///find MAX
                float value = vmaxnmvq_f32(abs);

                if (value > maxValue)
                    maxValue = value;
            }

            ///If size is not 16 byte aligned
            if (aligment != 0) {
                for (size_t i = VectorSize - aligment; i < VectorSize; ++i) {
                    Type absValue = std::abs(_elements[i]);
                    if (absValue > maxValue) {
                        maxValue = absValue;
                    }
                }
            }

            float coefficient = amplitude / maxValue;

            if (coefficient >= 1)
                return;

            for (size_t i = 0; i < interationCount; i++)
                *((float32x4_t *) &(_elements[i * sizeof(Type)])) =
                        vmulq_n_f32(*((float32x4_t *) &(_elements[i * sizeof(Type)])), coefficient);

            ///If size is 16 byte aligned
            if (aligment == 0)
                return;

            for (size_t i = VectorSize - aligment; i < VectorSize; ++i) {
                _elements[i] *= coefficient;
            }

            return;
        }

        ///if using other types

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

#endif