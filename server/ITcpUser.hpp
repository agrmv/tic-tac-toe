//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_SERVER_ITCPUSER_HPP
#define TICTACTOE_SERVER_ITCPUSER_HPP

#include <iostream>

class ITcpUser {
public:
    virtual ssize_t recv(void* buffer, size_t max_size) const = 0;
    virtual ssize_t send(void* buffer, size_t max_size) const = 0;
};

#endif //TICTACTOE_SERVER_ITCPUSER_HPP
