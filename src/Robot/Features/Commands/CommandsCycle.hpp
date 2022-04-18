#ifndef ROBOT_COMMANDSCYCLE_HPP
#define ROBOT_COMMANDSCYCLE_HPP

#include <mutex>
#include <thread>
#include <Math/Math.hpp>
#include "../Sensors/Sensors.hpp"
#include "../RobotSettings/RobotSettings.hpp"
#include "../MotorsSender/IMotorsSender.hpp"


#include "Stabilization.hpp"

namespace app {
    struct CommandsStruct {
        enum MoveDimensionsEnum : uint8_t {
            Fx = 0,
            Fy = 1,
            Fz = 2,
            Mx = 3,
            My = 4,
            Mz = 5
        };

        enum StabilizationTypeEnum : int8_t {
            None = 0,
            Course = 1,
            Absolute = 2
        };

        stc::VerticalVector<float, 6> Move{};
        std::array<float, 6> Hand{};
        std::array<float, 4> LowPWM{};

        StabilizationTypeEnum Stabilization = None;

        friend std::ostream &operator<<(std::ostream &output, const CommandsStruct &commands) {
            output << "[COMMANDS STRUCT]\n"
                   << "Move vector: "
                   << commands.Move[0] << ", "
                   << commands.Move[1] << ", "
                   << commands.Move[2] << ", "
                   << commands.Move[3] << ", "
                   << commands.Move[4] << ", "
                   << commands.Move[5] << ", "
                   << '\n'
                   << "Hand vector: "
                   << commands.Hand[0] << ", "
                   << commands.Hand[1] << ", "
                   << commands.Hand[2] << ", "
                   << commands.Hand[3] << ", "
                   << commands.Hand[4] << ", "
                   << commands.Hand[5] << ", "
                   << '\n'
                   << "LowPWM vector: "
                   << commands.LowPWM[0] << ", "
                   << commands.LowPWM[1] << ", "
                   << commands.LowPWM[2] << ", "
                   << commands.LowPWM[3];

            return output;
        }
    };


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

        PIDArray<3> pids_;
        CommandsQueue commandsQueue_;
        std::thread thread_;
        MotorsSender::IMotorsSender *motorsSender_;
        std::shared_ptr<Sensors> sensors_;
        std::shared_ptr<RobotSettings> settings_;
        std::atomic<bool> isNotDone_;
        Timer timer_;
        Stabilization stabilization_;
    public:

        CommandsCycle(MotorsSender::IMotorsSender *motorsSender,
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
