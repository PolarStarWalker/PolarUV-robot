#include <wait.h>
#include <fstream>
#include "Video.hpp"
#include "./VideoMessage.pb.h"

using namespace app;

Video::Video(ssize_t id) : IService(id), childPid_() {}

inline void Video::StartVideo(const std::string &pipeline) {

    {
        std::fstream file("pipeline", std::ios::out | std::ios::trunc);
        file << "#!/bin/bash" << std::endl;
        file << "gst-launch-1.0 -v -e ";
        file << pipeline;
    }

    std::system("chmod +x pipeline");

    if (childPid_ != 0)
        KillStream();

    pid_t pid = fork();

    if (pid == 0) {
        setpgid(0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        execlp("bash", "bash", "pipeline", nullptr);
    }

    childPid_ = pid;
}

lib::network::Response Video::Write(std::string_view &action) {
    VideoMessage message;
    message.ParseFromArray(action.begin(), action.size());

    switch (message.action()) {
        case VideoMessage::START:
            StartVideo(message.pipeline());
            break;
        case VideoMessage::STOP:
            KillStream();
            break;
    }

    return {"", lib::network::Response::NoContent, serviceId_};
}

void Video::KillStream() {
    if (childPid_ != 0) {
        kill(childPid_, SIGTERM);
        usleep(50 * 1000);
        kill(childPid_, SIGKILL);
        waitpid(childPid_, nullptr, 0);
    }

    childPid_ = 0;
}
