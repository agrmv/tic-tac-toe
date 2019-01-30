//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_SERVER_CGRID_HPP
#define TICTACTOE_SERVER_CGRID_HPP

#include <iostream>
#include <mutex>
#include "COutPacket.hpp"

class CGrid {
    ePlayer motion { ePlayer::PLAYER_X };
    eWinner winner { eWinner::UNKNOWN };
    eCellType grid[9] { eCellType::CELL_EMPTY };

    bool validateData(ePlayer player, int cell_id) {
        return cell_id >= 0 && cell_id < 9 && player == motion && grid[cell_id] == eCellType::CELL_EMPTY;
    }

    void checkWinner(int cell_id) {
        if (grid[cell_id] == eCellType::CELL_EMPTY)
            return;
        auto line = cell_id / 3, col = cell_id % 3;
        if (horizontal(line) || vertical(col) || diagonal(line, col)) {
            winner = (grid[cell_id] == eCellType::CELL_X ? eWinner::PLAYER_X : eWinner::PLAYER_O);
        } else {
            for (const auto &cell : grid)
                if (cell == eCellType::CELL_EMPTY)
                    return;
            winner = eWinner::DEAD_HEAT;
        }
    }

    bool horizontal(int line) const {
        auto start = line * 3;
        auto check = grid[start];
        for (auto end = start + 3; start < end; ++start)
            if (grid[start] != check)
                return false;
        return true;
    }

    bool vertical(int column) const {
        auto check = grid[column];
        for (auto start = column + 3, end = column + 9; start < end; start += 3)
            if (grid[start] != check)
                return false;
        return true;
    }

    bool mainDiagonal() const {
        auto check = grid[0];
        for (auto id = 4; id < 9; id += 4)
            if (grid[id] != check)
                return false;
        return true;
    }

    bool sideDiagonal() const {
        auto check = grid[6];
        for (auto id = 4; id > 0; id -= 2)
            if (grid[id] != check)
                return false;
        return true;
    }

    bool diagonal(int line, int column) const {
        auto main = (line == column), side = (column + line == 2);
        return (main && mainDiagonal()) || (side && sideDiagonal());
    }

public:

    bool setCell(ePlayer player, int cell_id) {
        static std::mutex mutex;
        std::lock_guard guard(mutex);
        if (!validateData(player, cell_id) || isWinnerExist())
            return false;
        if (motion == ePlayer::PLAYER_X) {
            grid[cell_id] = eCellType::CELL_X;
            motion = ePlayer::PLAYER_O;
        } else {
            grid[cell_id] = eCellType::CELL_O;
            motion = ePlayer::PLAYER_X;
        }
        checkWinner(cell_id);
        return true;
    }

    eWinner getWinner() const {
        return winner;
    }

    bool isWinnerExist() const {
        return getWinner() != eWinner::UNKNOWN;
    }

    COutPacket getPacket() const {
        return COutPacket { ePackets::UPDATE_GRID, reinterpret_cast<const uint8_t*>(grid), sizeof(grid) };
    }
};

#endif //TICTACTOE_SERVER_CGRID_HPP
