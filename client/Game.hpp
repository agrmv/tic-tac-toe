//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_CLIENT_GAME_HPP
#define TICTACTOE_CLIENT_GAME_HPP

#include <SFML/Network/IpAddress.hpp>
#include <getopt.h>
#include "Menu.hpp"
#include "NetworkClient.hpp"
#include "OnlineRender.hpp"
#include "Wait.hpp"

class Config {
    sf::IpAddress address { sf::IpAddress::LocalHost };
    unsigned short port { 7777 };
public:
    void parse(int argc, char** argv) {
        int res = 0;
        while ((res = getopt(argc, argv, "i:p:")) != -1) {
            switch (res) {
                case 'i':
                    address = sf::IpAddress(optarg);
                    break;
                case 'p':
                    port = uint16_t(atoi(optarg));
                    break;
            };
        };
    }

    auto getAddress() const {
        return address;
    }

    auto getPort() const {
        return port;
    }
};

class Game {
    sf::RenderWindow mainWindow;
    std::shared_ptr<RenderLayer> current;
    std::shared_ptr<Menu> menu;
    std::shared_ptr<Wait> wait;
    sf::Texture background;
    sf::Sprite sprite;
//    Menu;

    //2:1
    auto calculationWindowSize() const {
        return sf::Vector2u{
                sf::VideoMode::getDesktopMode().width / 2,
                sf::VideoMode::getDesktopMode().width / 4
        };
    }

    auto calculationPositionOnCenter() const {
        auto size = calculationWindowSize();
        return sf::Vector2u{
                sf::VideoMode::getDesktopMode().width / 2 - size.x / 2,
                sf::VideoMode::getDesktopMode().height / 2 - size.y / 2
        };
    }

    void initBackground(const sf::String& image = resources::textures::Background) {
        background.loadFromFile(image);
        background.setSmooth(true);

        sf::Vector2f size(mainWindow.getSize());
        sf::Vector2f textureSize(background.getSize());
        sprite.setTexture(background);
        sprite.scale(size.x / textureSize.x, size.y / textureSize.y);
    }

public:
    void loadIcon(sf::Window& window) {
        sf::Image icon;
        icon.loadFromFile(resources::textures::Icon);
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    void initWindow() noexcept {
        auto windowSize{calculationWindowSize()};
        auto windowPosition{calculationPositionOnCenter()};
        mainWindow.create(sf::VideoMode(windowSize.x, windowSize.y), "Tic Tac Toe", sf::Style::Close);
        loadIcon(mainWindow);
        mainWindow.setPosition(sf::Vector2i(windowPosition.x, windowPosition.y));
        initBackground();
    }

    void initMenu() {
        menu = std::make_shared<Menu>(mainWindow.getSize());
        menu->onExit([&] { mainWindow.close(); });
    }

    Game() {
        initWindow();
        initMenu();
        wait = std::make_shared<Wait>(mainWindow.getSize());
        current = menu;
    }

public:
    static Game& init() {
        static Game instance;
        return instance;
    }

    void run(const Config& config) noexcept {
        NetworkClient network;
        auto startHandler = network.createHandler(ePackets::START_GAME, [&](const CInPacket& packet) {
            auto player = *reinterpret_cast<const ePlayer *>(packet.getBody());
            current = std::make_shared<OnlineRender>(mainWindow.getSize(), network, player);
        });
        auto stopHandler = network.createHandler(ePackets::STOP_GAME, [&](const CInPacket& packet) {
            const auto& winner = *reinterpret_cast<const eWinner*>(packet.getBody());
            switch (winner) {
                case eWinner::PLAYER_X: menu->setWinner("The game ended in victory for the player: X"); break;
                case eWinner::PLAYER_O:  menu->setWinner("The game ended in victory for the player: O"); break;
                case eWinner::DEAD_HEAT:  menu->setWinner("The game ended in a draw"); break;
                default: menu->setWinner(""); break;
            }
            current = menu;
            network.stop();
        });

        menu->onPlay([&] {
            current = wait;
            network.run(config.getAddress(), config.getPort());
        });

        sf::Event event{};
        while (mainWindow.isOpen()) {
            while (mainWindow.pollEvent(event)) {
                current->event(event);
                if (event.type == sf::Event::Closed)
                    mainWindow.close();
            }
            mainWindow.draw(sprite);
            mainWindow.draw(*current);
            mainWindow.display();
        }

        network.deleteHandler(startHandler);
        network.deleteHandler(stopHandler);
    }
};

#endif //TICTACTOE_CLIENT_GAME_HPP