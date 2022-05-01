#include "./Startup.hpp"

using namespace app;


int main() {

    lib::Initialize();

    Robot robot;
    robot.SetUp();
    robot.Start();

    return 0;
}