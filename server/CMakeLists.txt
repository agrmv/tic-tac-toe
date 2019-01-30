cmake_minimum_required(VERSION 3.11)
project(TicTacToe-server)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp shared_container.hpp shared_memory.hpp CPacket.hpp CInPacket.hpp COutPacket.hpp ITcpUser.hpp CPlayer.hpp CPlayers.hpp CGrid.hpp CGame.hpp)

target_link_libraries(${PROJECT_NAME} -pthread)