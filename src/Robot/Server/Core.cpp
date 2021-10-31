#include "./Core/Core.hpp"

using namespace Server;


constexpr size_t HeaderSize = sizeof(Header);

Core::Core(uint16_t port) : _socket(port) {
    thread = std::thread(&Core::Run, this);
    thread.detach();
}

void Core::Run() {
    for (;;) {
        _socket.Listen();

        while (_socket.IsOnline()) {

            std::byte rawHeader[HeaderSize]{};

            if (_socket.RecvDataLen((char *) rawHeader, HeaderSize) != HeaderSize)
                break;

            auto &header = (Header &) rawHeader;

            if (header._destinationId == -1) {
                Header response(Connection, 0, -1);
                _socket.SendDataLen((char *) &response, HeaderSize);
            }
        }

    }
}
