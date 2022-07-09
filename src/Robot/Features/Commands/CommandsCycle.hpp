#ifndef ROBOT_COMMANDSCYCLE_HPP
#define ROBOT_COMMANDSCYCLE_HPP

#include <mutex>
#include <thread>
#include <Math/Math.hpp>
#include "../Sensors/Sensors.hpp"
#include "../RobotSettings/RobotSettings.hpp"
#include "../MotorsSender/IMotorsSender.hpp"
#include "CommandsStruct.hpp"

#include "Stabilization.hpp"

namespace app {

    class CommandsCycle {

        constexpr static size_t PERIOD_US = 2500;

        class CommandsQueue {
            constexpr static size_t MAX_COUNT = 400;
        public:
            std::mutex commandsMutex_;
            CommandsStruct commands_;
            size_t requestsCount_;
        public:
            CommandsQueue() : commands_(), requestsCount_(0) {};

            inline CommandsStruct GetCommands() {
                std::lock_guard guard(commandsMutex_);

                ++requestsCount_;

                if(requestsCount_ == MAX_COUNT)
                    commands_ = CommandsStruct();

                return commands_;
            }

            inline void AddCommands(const CommandsStruct& commandsStruct){
                std::lock_guard guard(commandsMutex_);
                requestsCount_ = 0;
                commands_ = commandsStruct;
            }
        };

        CommandsQueue commandsQueue_;
        MotorsSender::IMotorsSender &motorsSender_;
        std::shared_ptr<Sensors> sensors_;
        std::shared_ptr<RobotSettings> settings_;

        Timer timer_;
        Stabilization stabilization_;
        PIDArray<4> pids_;

        std::atomic<bool> isNotDone_;
        std::thread thread_;

    public:

        CommandsCycle(MotorsSender::IMotorsSender &motorsSender,
                      std::shared_ptr<Sensors> sensors,
                      std::shared_ptr<RobotSettings> settings);

        ~CommandsCycle();

        void UpdateCommands(const CommandsStruct& commandsStruct) {
            commandsQueue_.AddCommands(commandsStruct);
        }

    private:

        void StartCommands();

        CommandsStruct GetCommands(){
            return commandsQueue_.GetCommands();
        }

    };

}

#endif
