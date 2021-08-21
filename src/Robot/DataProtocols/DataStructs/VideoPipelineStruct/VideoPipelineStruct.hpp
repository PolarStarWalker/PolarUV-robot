#ifndef CLIENT_VIDEOSTRUCT_HPP
#define CLIENT_VIDEOSTRUCT_HPP
#include <cstdio>

class VideoPipelineStruct {
private:
    char *_data;

public:
    VideoPipelineStruct(size_t size);
    ~VideoPipelineStruct();
    size_t Size();
    char* Begin();
    char* StringBegin();
};

#endif
