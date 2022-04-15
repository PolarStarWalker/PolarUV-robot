#ifndef ROBOT_BUFFERTYPE_HPP
#define ROBOT_BUFFERTYPE_HPP

#include <zstd.h>
#include <array>
#include <cstdint>
#include "Exceptions/Exceptions.hpp"

class ArrayView {
    char *data_;
    size_t size_;

public:

    ArrayView(char *data, size_t size) : data_(data), size_(size) {};

    [[nodiscard]] char *begin() & noexcept { return data_; }

    [[nodiscard]] const char *cbegin() const & noexcept { return data_; }

    [[nodiscard]] size_t size() const noexcept { return size_; }

    [[nodiscard]] const char &operator[](size_t index) const & {
#ifdef DEBUG
        if (index > size_)
            throw std::out_of_range("index out of range");
#endif
        return data_[index];
    }

    explicit operator std::string_view(){
        return {data_, size_};
    }

};


template<size_t Size, size_t MaxRatio>
class BufferType {

    ///ToDo: Make it consteval when clang-tidy is ready
    constexpr static size_t Capacity() { return (MaxRatio + 1) * Size; }

    std::array<char, Capacity()> data_;

public:

    BufferType() : data_() {}

    [[nodiscard]] inline ArrayView Decompress(size_t size) {

        auto decompressedSize = ZSTD_getFrameContentSize(data_.data(), size);

        if(decompressedSize > Capacity() - size)
            throw lib::exceptions::BufferOverflow("Decompress size more then buffer");

        char* dst = data_.begin() + size;
        decompressedSize = ZSTD_decompress(dst, Capacity() - size, &data_[0], size);


        return {dst, decompressedSize};
    }

    [[nodiscard]] inline ArrayView Compress(const char* src, size_t srcSize) &{
        auto maxSize = ZSTD_compressBound(srcSize);
        if(maxSize > Capacity())
            throw lib::exceptions::BufferOverflow("Compress size more then capacity of buffer");

        auto dstSize = ZSTD_compress(&data_[0], Capacity(), src, srcSize, 0);

        return {data_.data(), dstSize};
    }

    char *begin() noexcept { return data_.begin(); }

    explicit operator std::array<char, Size>(){
        return *((std::array<char, Size>*) data_.begin());
    }

    [[nodiscard]] char* data() & noexcept{ return data_.begin();}
    [[nodiscard]] const char* data() const & noexcept{ return data_.cbegin();}
};


#endif
