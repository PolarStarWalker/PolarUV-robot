#ifndef POLAR_UV_COMMANDSSTRUCT_HPP
#define POLAR_UV_COMMANDSSTRUCT_HPP
struct CommandsStruct {
    enum MoveDimensionsEnum : uint8_t {
        Vx = 0,
        Vy = 1,
        Vz = 2,
        Wx = 3,
        Wy = 4,
        Wz = 5
    };

    stc::VerticalVector<float, 6> Move{};
    std::array<float, 6> Hand{};
    std::array<float, 4> LowPWM{};

    bool Stabilize = false;

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
#endif //POLAR_UV_COMMANDSSTRUCT_HPP
