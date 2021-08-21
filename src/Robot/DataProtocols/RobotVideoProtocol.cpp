#include <chrono>
#include <thread>
#include <csignal>
#include <wait.h>
#include "RobotVideoProtocol/RobotVideoProtocol.hpp"
#include "DataStructs/VideoPipelineStruct/VideoPipelineStruct.hpp"

RobotVideoProtocol::RobotVideoProtocol() {
    ///Port created by https://steamcommunity.com/profiles/76561198126802792/
    _socket.MakeServerSocket(28840);
    this->childPid = 0;
}

void RobotVideoProtocol::Start() {
    for (;;) {
        _socket.Listen();
        char action = 0;
        _socket.RecvDataLen(&action, 1);

        ///start
        if (action == 's') {

            ssize_t size = 0;

            if (_socket.RecvDataLen((char *) &size, sizeof(size)) == -1)
                continue;

            VideoPipelineStruct pipeline(size);

            _socket.RecvDataLen(pipeline.StringBegin(), size);

            std::fstream file("pipeline", std::ios_base::out | std::ios_base::trunc);
            file << "#!/bin/bash" << std::endl;
            file << "gst-launch-1.0 -v -e ";

            file.write(pipeline.StringBegin(), size);
            file.close();

            std::system("chmod +x pipeline");

            if (this->childPid != 0)
                this->KillStream();

            pid_t pid = fork();

            if (pid == 0) {
                setpgid(0, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                execlp("bash", "bash", "pipeline", nullptr);
            }

            this->childPid = pid;

            continue;
        }

        ///finish
        if (action == 'f') {

            this->KillStream();

            continue;
        }
    }
}

void RobotVideoProtocol::KillStream() {

    if (this->childPid != 0) {
        kill(-this->childPid, SIGTERM);
        usleep(50 * 1000);
        kill(-this->childPid, SIGKILL);
        waitpid(-this->childPid, nullptr, 0);
    }

    this->childPid = 0;
}

void RobotVideoProtocol::StartAsync() {
    std::thread thread(&RobotVideoProtocol::Start, this);
    thread.detach();
}

RobotVideoProtocol::~RobotVideoProtocol() {
    this->KillStream();
}


