#include "CommandsCycle.hpp"
#include <algorithm>
#include <ranges>
#include <iostream>
#include "Schedular/Event.hpp"

using namespace app;

inline MotorsSender::MotorsStruct FormMotorsStruct(const std::array<float, 12> &hiPwm,
                                                   const std::array<float, 4> &lowPwm) {

    auto func = [](float x) -> uint16_t { return std::ceil(x); };

    MotorsSender::MotorsStruct motors;

    std::ranges::transform(hiPwm.cbegin(), hiPwm.cend(), motors.HiPWM.begin(), func);

    std::ranges::transform(lowPwm.cbegin(), lowPwm.cend(), motors.LowPWM.begin(), func);

    return motors;
}

CommandsCycle::CommandsCycle(MotorsSender::IMotorsSender &motorsSender,
                             std::shared_ptr<Sensors> sensors,
                             std::shared_ptr<RobotSettings> settings) :
        motorsSender_(motorsSender),
        sensors_(std::move(sensors)),
        settings_(std::move(settings)),
        isNotDone_(true),
        thread_(&CommandsCycle::StartCommands, this) {}

CommandsCycle::~CommandsCycle() {
    isNotDone_.store(false);
    thread_.join();
}

void CommandsCycle::StartCommands() {

//    TimerType timer;
//    timer.SetTimer(TimerType::SleepFor_ms(1));
//    EventTracker eventTracker(10);
//    eventTracker.TrackEvent(timer, 0);

    while (isNotDone_.load()) {

        //auto events = eventTracker.Listen<10>(20);

        /// Расчет dt
        auto dt = timer_.Update();

        /// Получение команд
        auto commands = GetCommands();

        /// Получение настроек
        auto settings = settings_->GetSettings();

        /// Получение данных с датчиков
        auto sensors = sensors_->GetSensorsStruct();

        if (commands.Stabilization) {

            /// Установка коэффициентов
            stabilization_.SetPCoefficients(settings.PIDCoefficients.PArray);
            stabilization_.SetICoefficients(settings.PIDCoefficients.IArray);
            stabilization_.SetDCoefficients(settings.PIDCoefficients.DArray);

            /// Расчет значений векторов движения
            auto stabilizationMove = stabilization_.Calculate(dt, commands, settings, sensors);

            commands.Move[CommandsStruct::MoveDimensionsEnum::Vx] += stabilizationMove[0];
            commands.Move[CommandsStruct::MoveDimensionsEnum::Vy] += stabilizationMove[1];
            commands.Move[CommandsStruct::MoveDimensionsEnum::Vz] = stabilizationMove[2];
            commands.Move[CommandsStruct::MoveDimensionsEnum::Wx] += stabilizationMove[3];
            commands.Move[CommandsStruct::MoveDimensionsEnum::Wy] = stabilizationMove[4];
            commands.Move[CommandsStruct::MoveDimensionsEnum::Wz] += stabilizationMove[5];

        } else {
            /// Сброс параметров стабилизации
            stabilization_.Reset(sensors);
        }

        /// Применение глобальных коэффициентов
        auto hiPWM = settings.ThrustersCoefficientArray * commands.Move;
        hiPWM.Normalize(100.0f);

        /// Расчет команд манипулятора
        auto handBegin = commands.Hand.cbegin();
        auto handEnd = commands.Hand.cbegin() + settings.HandFreedom;
        std::transform(handBegin, handEnd,
                       settings.HandCoefficientArray.cbegin(),
                       hiPWM.rbegin(),
                       [](float hand, float coefficient) { return hand * coefficient; });

        /// Приведение команд от диапазона [-100; 100] к диапазону [0; 2000]
        hiPWM += 100.0f;
        hiPWM *= 10.0f;

        /// Формирование структуры команд
        auto motorsStruct = FormMotorsStruct((const std::array<float, 12> &) hiPWM, commands.LowPWM);

        /// Отправка команд на двигатели
        motorsSender_.SendMotorsStruct(motorsStruct);

        for (size_t i = 0; i < 8; i++) {
            std::cout << motorsStruct.HiPWM[i] << " ";
        }
        std::cout << std::endl;

        usleep(PERIOD_US);
    }
}