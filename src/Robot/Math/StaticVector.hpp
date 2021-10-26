#ifndef ROBOT_STATICVECTOR_HPP
#define ROBOT_STATICVECTOR_HPP


template<typename Type, size_t VectorSize> requires(std::is_arithmetic_v<Type>)
class StaticVector {
private:
    Type _elements[VectorSize]{};

public:

    StaticVector() = default;

    StaticVector(const Type array[VectorSize]) noexcept {
        for (size_t i = 0; i < VectorSize; i++)
            _elements[i] = array[i];

    }

    StaticVector(const StaticVector<Type, VectorSize> &vector) noexcept {
        for (size_t i = 0; i < VectorSize; i++)
            _elements[i] = vector._elements[i];

    }

    StaticVector(const Type * array, size_t rows) noexcept {
        for (size_t i = 0; i < rows; ++i)
            _elements[i] = array[i];
    }

    ~StaticVector() = default;

    size_t Size() const noexcept {return VectorSize;}

    Type &operator[](size_t index) { return _elements[index]; }

    const Type &operator[](size_t index) const { return _elements[index]; }

    StaticVector<Type, VectorSize> &operator=(const Type array[VectorSize]) {
        for (size_t i = 0; i < VectorSize; i++) {
            _elements[i] = array[i];
        }
        return *this;
    }

    StaticVector<Type, VectorSize> &operator*=(Type value) {
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
        for (size_t i = 0; i < VectorSize; ++i) {
            _elements[i] += value;
        }
        return *this;
    }

    void Normalize(Type amplitude) {
        double maxValue = 0;

        for (size_t i = 0; i < VectorSize; i++) {
            Type absValue = std::abs(_elements[i]);
            if (absValue > maxValue) {
                maxValue = absValue;
            };
        }

        double coefficient = amplitude / maxValue;

        if (coefficient < 1) {
            for (size_t i = 0; i < VectorSize; i++) {
                _elements[i] *= coefficient;
            }
        }
    }

    void FillArray(std::array<uint16_t, 12> *array, size_t offset = 0) const {
        for (size_t i = 0; i < VectorSize; ++i) {
            (*array)[i + offset] = _elements[i];
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