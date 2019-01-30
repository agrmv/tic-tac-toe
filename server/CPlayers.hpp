//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_SERVER_CPLAYERS_HPP
#define TICTACTOE_SERVER_CPLAYERS_HPP

#include "CPlayer.hpp"
#include "CGrid.hpp"

class CPlayers {
    CPlayer x;
    CPlayer o;
public:
    CPlayers(int socketx, int socketo) : x {socketx, ePlayer::PLAYER_X }, o {socketo, ePlayer::PLAYER_O } {
    }

    const CPlayer& getPlayerX() const {
        return x;
    }

    const CPlayer& getPlayerO() const {
        return o;
    }
};

#endif //TICTACTOE_SERVER_CPLAYERS_HPP
