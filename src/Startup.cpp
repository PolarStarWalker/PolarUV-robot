#include "./Startup.hpp"

using namespace app;

[[noreturn]]
int main() {
    Robot robot;
    robot.SetUp();
    robot.Start();
}