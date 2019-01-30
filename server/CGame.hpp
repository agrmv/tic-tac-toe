//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_SERVER_CGAME_HPP
#define TICTACTOE_SERVER_CGAME_HPP

#include <iostream>
#include "CPlayers.hpp"
#include "CGrid.hpp"
#include "CPlayer.hpp"
#include "CInPacket.hpp"
#include <thread>

class CGame {
    uint64_t m_id;
    CPlayers m_players;
    CGrid    m_grid;

    void broadcast(const COutPacket& packet) {
        packet.send(m_players.getPlayerX());
        packet.send(m_players.getPlayerO());
    }

    COutPacket stopGamePacket(eWinner winner = eWinner::UNKNOWN) const {
        if (winner == eWinner::UNKNOWN)
            winner = m_grid.getWinner();
        return COutPacket {ePackets::STOP_GAME, reinterpret_cast<uint8_t*>(&winner), sizeof(winner)};
    }

    COutPacket startGamePacket(ePlayer player) const {
        return COutPacket {ePackets::START_GAME, reinterpret_cast<uint8_t*>(&player), sizeof(player)};
    }

    void player_thread(const CPlayer& player, const CPlayer& enemy) {
        CInPacket packet;
        while (packet.read(player)) {
            switch (packet.getPacketType()) {
                case ePackets::CHAT: {
                    COutPacket(ePackets::CHAT, packet.getBody(), packet.getBodySize()).send(enemy);
                    break;
                }
                case ePackets::SELECT_CELL: {
                    int cell = *(int*)packet.getBody();//TODO: нормальное чтение с пакета
                    if (!m_grid.setCell(player.getType(), cell)) {
                        m_grid.getPacket().send(player);
                        continue;
                    }
                    broadcast(m_grid.getPacket());
                    if (m_grid.isWinnerExist())
                        broadcast(stopGamePacket());
                    break;
                }
                default: break;
            }

            packet.clear();
        }
        if (!m_grid.isWinnerExist()) { //если игрок отключился до того как игра завершилась
            stopGamePacket(eWinner::DEAD_HEAT).send(enemy);
        }
    }

public:
    CGame(int x, int o) : m_players{ x, o } {
        static uint64_t gen_id = 0;
        m_id = ++gen_id;
    }

    auto getId() const {
        return m_id;
    }

    auto getPlayers() const {
        return m_players;
    }

    bool operator==(uint64_t id) const {
        return getId() == id;
    }

    void startGame() {
        std::thread xlistener(&CGame::player_thread, this, std::cref(m_players.getPlayerX()), std::cref(m_players.getPlayerO()));
        std::thread olistener(&CGame::player_thread, this, std::cref(m_players.getPlayerO()), std::cref(m_players.getPlayerX()));

        std::cout << "Game started " << getId() << std::endl;
        startGamePacket(ePlayer::PLAYER_X).send(m_players.getPlayerX());
        startGamePacket(ePlayer::PLAYER_O).send(m_players.getPlayerO());

        xlistener.join();
        olistener.join();
        std::cout << "Game completed " << getId() << std::endl;
    }
};

#endif //TICTACTOE_SERVER_CGAME_HPP
