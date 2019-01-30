//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_SERVER_CPLAYER_HPP
#define TICTACTOE_SERVER_CPLAYER_HPP

#include "ITcpUser.hpp"
#include "CGrid.hpp"

void close_connection(int& socket, const char* reason = nullptr) {
    if (reason)
        send(socket, reason, strlen(reason), 0);
    close(socket);
    socket = 0;
}

class CPlayer : public ITcpUser {
    int socket;
    ePlayer type;
public:
    explicit CPlayer(int socket, ePlayer type) : socket(socket), type(type) {
    }

    auto getType() const {
        return type;
    }

    ssize_t recv(void* buffer, size_t max_size) const final {
        return ::recv(socket, buffer, max_size, 0);
    }

    ssize_t send(void* buffer, size_t max_size) const final {
        return ::send(socket, buffer, max_size, 0);
    }

    void disconnect(const std::string& reason) {
        close_connection(socket, reason.c_str());
    }
};

#endif //TICTACTOE_SERVER_CPLAYER_HPP
