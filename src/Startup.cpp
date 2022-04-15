#include "./Startup.hpp"

using namespace app;


int main() {
    Robot robot;
    robot.SetUp();
    robot.Start();

    return 0;
}