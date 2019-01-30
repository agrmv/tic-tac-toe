//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_CLIENT_CCLIENT_HPP
#define TICTACTOE_CLIENT_CCLIENT_HPP

#include "ITcpUser.hpp"
#include <iostream>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/System.hpp>

class CClient : public ITcpUser {
    mutable sf::TcpSocket socket;

public:
    void connect(const sf::IpAddress& address, uint16_t port) const {
        socket.setBlocking(false);
        socket.connect(address, port);
    }

    void disconnect() const {
        socket.disconnect();
    }

    ssize_t recv(void *buffer, size_t max_size) const override {
        decltype(sf::TcpSocket::Done) code;
        do {
            size_t read{};
            code = socket.receive(buffer, max_size, read);
            if (code == sf::TcpSocket::Done)
                return read;
            sf::sleep(sf::milliseconds(5));
        } while (code != sf::TcpSocket::Disconnected && code != sf::TcpSocket::Error);
        return 0;
    }

    ssize_t send(void *buffer, size_t max_size) const override {
        size_t write{};
        socket.send(buffer, max_size, write);
        return write;
    }
};

#endif //TICTACTOE_CLIENT_CCLIENT_HPP