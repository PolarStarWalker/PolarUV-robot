#ifndef ROBOT_ROBOTSETTINGS_HPP
#define ROBOT_ROBOTSETTINGS_HPP

#include <mutex>
#include "Math/Static/Vector.hpp"
#include "Math/Static/Matrix.hpp"
#include <TcpSession/TcpSession.hpp>

namespace app {

    template<size_t Size>
    struct PIDCoefficientsArray {
        std::array<float, Size> PArray{};
        std::array<float, Size> IArray{};
        std::array<float, Size> DArray{};
    };

    struct RobotSettingsStruct {

        using Matrix_t = stc::Matrix<float, 12, 6>;
        using Vecotr_t = stc::Vector<stc::Horizontal, float, 6>;

        Matrix_t ThrustersCoefficientArray{};
        Vecotr_t HandCoefficientArray{};
        size_t ThrustersNumber = 0;
        size_t HandFreedom = 0;
        PIDCoefficientsArray<4> PIDCoefficients{};

        friend std::ostream &operator<<(std::ostream &out, const RobotSettingsStruct &settings) {

            out << "[MOTORS SETTINGS]\n"
                << "ThrustersNumber: " << settings.ThrustersNumber << '\n'
                << "ThrustersCoefficients:\n"
                << settings.ThrustersCoefficientArray << '\n'
                << "HandFreedom: " << settings.HandFreedom << '\n'
                << "HandCoefficients: " << settings.HandCoefficientArray /*<< '\n'
                << "X - PID " << settings.PIDCoefficients[0] << '\n'
                << "Y - PID " << settings.PIDCoefficients[1] << '\n'
                << "Z - PID " << settings.PIDCoefficients[2] << '\n'
                << "Depth - PID " << settings.PIDCoefficients[3]*/;

            return out;
        }
    };

    class RobotSettings final : public lib::network::IService {
        using Response = lib::network::Response;
    public:

        RobotSettings(const RobotSettings &) = delete;

        RobotSettings(RobotSettings &&) = delete;

        RobotSettings &operator=(const RobotSettings &) = delete;

        RobotSettings &operator=(RobotSettings &&) = delete;

        RobotSettings(ssize_t id, std::string_view filename);

        bool WriteValidate(const std::string_view &robotSettings) final;

        void Write(const std::string_view &robotSettings) final;

        ResponseBufferType Read() final;

        RobotSettingsStruct GetSettings() const;

    private:
        void SetSettings(const RobotSettingsStruct &settingsData);

        mutable std::mutex settingsMutex_;
        std::string_view filename_;
        RobotSettingsStruct settings_;
    };

}
#endif