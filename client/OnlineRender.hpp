//
// Created by root on 19.12.18.
//

#ifndef PROJECT_ONLINERENDER_HPP
#define PROJECT_ONLINERENDER_HPP

#include "RenderLayer.hpp"
#include "ChatRender.hpp"
#include "GridRender.hpp"
#include <cstring>

class OnlineRender : public RenderLayer {
    NetworkClient& client;
    ChatRender m_chat;
    mutable DefaultGrid m_grid;
    std::deque<NetworkClient::handler_identifier> ids;

    void draw(sf::RenderTarget& render) const override {
        render.draw(m_chat);
        render.draw(m_grid);
    }

public:
    explicit OnlineRender(const sf::Vector2u& size, NetworkClient& client, ePlayer player)
            : RenderLayer(size), client { client }, m_chat { size, player }, m_grid( { size.x / 2 / 3, size.y / 3 })  {
        m_grid.setPosition(static_cast<float>(size.x) / 2, 0);

        m_grid.setClickCell([&](int id) {
            client.send(COutPacket(ePackets::SELECT_CELL, (const uint8_t*)&id, sizeof(id)));
        });

        m_chat.sendMessage([&](const std::basic_string<sf::Uint8>& str) {
            client.send(COutPacket(ePackets::CHAT, str.data(), str.size()));
        });

        ids.emplace_back(client.createHandler(ePackets::CHAT, [&](const CInPacket& packet) {
            m_chat.addChat(sf::String::fromUtf8(packet.getBody(), packet.getBody() + packet.getBodySize()));
        }));

        ids.emplace_back(client.createHandler(ePackets::UPDATE_GRID, [&](const CInPacket& packet) {
           eCellType grid[DefaultGrid::getGridSize()];
           memcpy(grid, packet.getBody(), DefaultGrid::getGridSize());//min body size/grid size
           m_grid.updateGrid(grid);
        }));
    }

    void event(const sf::Event& event) override {
        m_chat.event(event);
        m_grid.event(event);
    }

    ~OnlineRender() final {
        for (auto& id : ids)
            client.deleteHandler(id);
    }
};


#endif //PROJECT_ONLINERENDER_HPP