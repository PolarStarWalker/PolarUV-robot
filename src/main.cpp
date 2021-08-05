#include "main.hpp"


int main() {
    ///set max sched priority
    struct sched_param process{};
    process.sched_priority = 99;
    sched_setscheduler(0, SCHED_RR, &process);

    ///Set program
    ///structs for transfer data
    CommandsProtocol commandsProtocol("/dev/spidev0.0");

    commandsProtocol.StartAsync();

    for(;;){
        ///ToDo: повесить сюда какие-нибудь действия
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}