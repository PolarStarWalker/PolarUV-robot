#include "Commands.hpp"

using namespace app;

CommandsService::CommandsService(ssize_t id) : lib::network::IService(id){}

lib::network::Response CommandsService::WriteRead(std::string_view &data) {


    return IService::WriteRead(data);
}
