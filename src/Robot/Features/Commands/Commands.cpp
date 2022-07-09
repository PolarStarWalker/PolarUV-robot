#include "Commands.hpp"
#include "CommandsCycle.hpp"

using namespace app;

CommandsService::CommandsService(ssize_t id,
                                 MotorsSender::IMotorsSender &motorsSender,
                                 std::shared_ptr<Sensors> sensors,
                                 std::shared_ptr<RobotSettings> settings) :
        lib::network::IService(id),
        commandsCycle_(motorsSender, std::move(sensors), std::move(settings)) {}

void CommandsService::Write(const std::string_view &data) {

    if(data.size() != sizeof(CommandsStruct))
        throw lib::exceptions::TransferError("data.size() and sizeof(CommandsStruct) mismatch");

    auto &commands = *((const CommandsStruct *) data.data());

    //std::cout << commands << std::endl;

    commandsCycle_.UpdateCommands(commands);
}






