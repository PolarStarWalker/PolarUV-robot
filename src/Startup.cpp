#include "./Startup.hpp"

using namespace app;


int main() {

    lib::Initialize();

    try {

        Robot robot;
        robot.SetUp();
        robot.Start();

    } catch (lib::exceptions::BaseException &e) {
        std::cout << "[APPLICATION EXCEPTION]"
                  << '\n' << e << std::endl;

    } catch (std::exception &e) {
        std::cout << "[UNKNOWN EXCEPTION]"
                  << '\n' << e.what() << std::endl;

    } catch (...) {
        std::cout << "[UNKNOWN ERROR, TERMINATE]" << std::endl;
    }

    return 0;
}