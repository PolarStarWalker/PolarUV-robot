#ifndef ROBOT_STATICVECTOR_HPP
#define ROBOT_STATICVECTOR_HPP


template<typename Type, size_t Size> requires(std::is_arithmetic_v<Type>)
class StaticVector {
private:
    Type _elements[Size]{};

public:

    StaticVector() = default;

    StaticVector(const StaticVector<Type, Size> &vector) noexcept {
        for (size_t i = 0; i < Size; i++)
            _elements[i] = vector._elements[i];

    }

    ~StaticVector() = default;

    Type &operator[](size_t index) { return _elements[index]; }

    const Type &operator[](size_t index) const { return _elements[index]; }

    StaticVector<Type, Size> &operator=(const Type *array) {
        for (size_t i = 0; i < Size; i++) {
            _elements[i] = array[i];
        }
        return *this;
    }

    StaticVector<Type, Size> &operator*=(Type value) {
        for (size_t i = 0; i < Size; ++i) {
            _elements[i] *= value;
        }
        return *this;
    }

    StaticVector<Type, Size> &operator/=(Type value) {
        for (size_t i = 0; i < Size; ++i) {
            _elements[i] /= value;
        }
    }

    StaticVector<Type, Size> &operator+=(Type value) {
        for (size_t i = 0; i < Size; ++i) {
            _elements[i] += value;
        }
        return *this;
    }

    void Normalize(Type amplitude) {
        double maxValue = 0;

        for (size_t i = 0; i < Size; i++) {
            Type absValue = std::abs(_elements[i]);
            if (absValue > maxValue) {
                maxValue = absValue;
            };
        }

        double coefficient = amplitude / maxValue;

        if (coefficient < 1) {
            for (size_t i = 0; i < Size; i++) {
                _elements[i] *= coefficient;
            }
        }
    }

    void FillArray(std::array<uint16_t, 12> *array, size_t offset = 0) const {
        for (size_t i = 0; i < Size; ++i) {
            (*array)[i + offset] = _elements[i];
        }
    }

    friend std::ostream &operator<<(std::ostream &stream, const StaticVector<Type, Size> &vector) {

        stream << '[';
        for (ssize_t i = 0; i < Size - 1; i++) {
            stream << vector[i] << ", ";
        }
        stream << vector[Size - 1];
        stream << "]\n";

        return stream;
    };

};

#endif