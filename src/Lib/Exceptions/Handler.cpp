#include <functional>
#include <string_view>
#include "Handler.hpp"

using namespace lib::exceptions;
using Response = lib::network::Response;

Response Handler(std::function<Response(std::string_view)>){
    
}