#include <wait.h>
#include <fstream>
#include "Video.hpp"
#include "./VideoMessage.pb.h"

using namespace app;


namespace auv {
    constexpr std::string_view auv_pipeline = R"(v4l2src device=/dev/video2 do-timestamp=true
        ! video/x-h264,width=1920,height=1080,framerate=30/1
        ! h264parse
        ! v4l2h264dec
        ! autovideoconvert
        ! appsink sync=false)";

    constexpr std::string_view auv_stream = R"(appsrc
        ! videoconvert
        ! v4l2h264enc output-io-mode=4  extra-controls="encode,frame_level_rate_control_enable=1,h264_profile=0,h264_level=14,video_bitrate=2500000;"
        ! h264parse
        ! matroskamux
        ! filesink location=/home/pi/files/test.mkv)";
}

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
        ///Create group for child process and his childs
        setpgid(0, 0);
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
        ///Kill child group
        killpg(childPid_, SIGKILL);
        waitpid(childPid_, nullptr, 0);
    }

    childPid_ = 0;
}
