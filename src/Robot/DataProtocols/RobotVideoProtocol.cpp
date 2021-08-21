#include <chrono>
#include <thread>
#include "RobotVideoProtocol/RobotVideoProtocol.hpp"
#include "DataStructs/VideoPipelineStruct/VideoPipelineStruct.hpp"

RobotVideoProtocol::RobotVideoProtocol() {
    ///Port created by https://steamcommunity.com/profiles/76561198126802792/
    _socket.MakeServerSocket(28840);
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

            this->childPid = fork();
            if (this->childPid == 0) {

                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                execlp("pipeline", nullptr);
            }
            continue;
        }

        ///finish
        if (action == 'f') {

            continue;
        }
    }
}

void RobotVideoProtocol::StartAsync() {
    std::thread thread(&RobotVideoProtocol::Start, this);
    thread.detach();
}


