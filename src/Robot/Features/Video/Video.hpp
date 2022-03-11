#ifndef ROBOT_VIDEO_HPP
#define ROBOT_VIDEO_HPP

#include <linux/videodev2.h>

#include "./DataTransmissions/TcpSession/TcpSession.hpp"

namespace app {

    class Video final : public lib::network::IService {
    public:

        Video(const Video&) = delete;
        Video(Video&&) = delete;
        Video& operator=(const Video&) = delete;
        Video& operator=(Video&&) = delete;

        explicit Video(ssize_t id);
        ~Video();
        Response Write(std::string_view &action) final;

    private:
        void StartVideo(const std::string&);
        void KillStream();

        pid_t childPid_{};
    };
}

#endif
