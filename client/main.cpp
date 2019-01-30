#include <fstream>
#include "Game.hpp"
#include "Wait.hpp"

int main(int argc, char** argv) {
    freopen("client.log", "a", stderr);
    fprintf(stderr, "\n###### START CLIENT (BUILD FROM %s - %s) ######\n", __DATE__, __TIME__);
    fprintf(stderr, "\n###### LOCAL TIME  ######\n"); //TODO: time
    fflush(stderr);

    Config config;
    config.parse(argc, argv);
    std::clog << "Connect to server (" << config.getAddress().toString() << " : " << config.getPort() << ")" << std::endl;
    
    auto& game = Game::init();
    game.run(config);
}

/*
 * TODO:
 *
 * 1) Меню через колбэки
 * 2) Сетевой клиент инициализуется в игровом классе после чего передаётся по DI
 * 3) Ожидание является промежуточным этапом, ожидает пока по сети не придёт инфа о начале
 * 4) Игра завершается приходом колбэка о завершение игры в класс игры
 * 5) Удаление триггеров
 *
 */