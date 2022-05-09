#include <functional>
#include <string_view>
#include "Handler.hpp"

using namespace lib::exceptions;
using Response = lib::network::Response;

Response Handler(const std::function<Response(std::string_view)>& f, std::string_view data){
    return f(data);
}