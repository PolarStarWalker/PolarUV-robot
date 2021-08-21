
#include "./VideoPipelineStruct/VideoPipelineStruct.hpp"

VideoPipelineStruct::VideoPipelineStruct(size_t size) {
    this->_data = new char[size + 8];
}

VideoPipelineStruct::~VideoPipelineStruct() {
    delete[] this->_data;
}

size_t VideoPipelineStruct::Size() {
    return *((size_t *) this->_data);
}

char *VideoPipelineStruct::Begin() {
    return this->_data;
}

char *VideoPipelineStruct::StringBegin() {
    return this->_data + sizeof(size_t);
}
