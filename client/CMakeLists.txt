cmake_minimum_required(VERSION 3.11)
project(TicTacToe-client)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE data resources/*)
file(COPY ${data} DESTINATION resources)

add_executable(${PROJECT_NAME} ${data} main.cpp GridCell.hpp RenderLayer.hpp GridRender.hpp ChatRender.hpp OnlineRender.hpp NetworkPackets.hpp resources.hpp CClient.hpp NetworkClient.hpp MenuItem.hpp StringMenuItem.hpp Menu.hpp Game.hpp GridTextures.hpp ITcpUser.hpp CPacket.hpp CInPacket.hpp COutPacket.hpp Wait.hpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

find_package(SFML CONFIG REQUIRED COMPONENTS graphics window system)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-system sfml-window sfml-network pthread)

include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})