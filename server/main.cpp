#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "shared_container.hpp"
#include "CGame.hpp"

void parseConfig(int argc, char** argv, uint16_t& games, uint16_t& port) {
    int res = 0;
    while ( (res = getopt(argc,argv,"m:p:")) != -1){
        switch (res){
            case 'm': games = uint16_t(atoi(optarg)); break;
            case 'p': port = uint16_t(atoi(optarg)); break;
        };
    };
}

int main(int argc, char** argv) {

    uint16_t maxGames = 4;
    uint16_t port = 7777;

    parseConfig(argc, argv, maxGames, port);
    std::cout << "Server start. Port: " << port << " Mac game count: " << maxGames << std::endl;

    struct sockaddr_in addr{
            .sin_family = AF_INET,
            .sin_port = htons(port),
            .sin_addr = {.s_addr = INADDR_ANY}
    };

    auto listener = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(listener, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(2);
    }

    listen(listener, maxGames * 2);
    auto& games = shared_memory::array<CGame>::create(maxGames);
    int waited = 0;

    while (true) {
        auto player = accept(listener, nullptr, nullptr);
        if (games.is_full()) {
            close_connection(player, "Max game count!\n");
            continue;
        }

        if (waited) {
            auto current_id = games.emplace_back(waited, player).getId();
            if (fork() == 0) {
                close(listener);
                auto id = games.find([&current_id](const CGame &r) {
                    return r == current_id;
                });

                games[id].startGame();

                id = games.find([&current_id](const CGame &r) {
                    return r == current_id;
                });
                games.erase(id);

                _exit(0);
            } else {
                close_connection(player);
                close_connection(waited);
            }
            continue;
        }
        waited = player;
    }

    close(listener);
    return 0;
}