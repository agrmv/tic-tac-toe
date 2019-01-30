//
// Created by root on 19.12.18.
//

#ifndef PROJECT_GRIDRENDER_HPP
#define PROJECT_GRIDRENDER_HPP

#include "GridTextures.hpp"
#include "RenderLayer.hpp"

//TODO: изменить структуру (возможно использовать позднею инициализию, изменить наследования)

template <uint size>
class GridRender : protected GridTextures, public RenderLayer {
    inline const auto& getTextureSize() const { return GridTextures::getSize<uint>(); };
public:
    inline static constexpr auto getGridSize() { return size * size; }
    using click_handler = std::function<void(int)>;

    explicit GridRender(const sf::Vector2u& cellSize = {})
            : RenderLayer({size * GridTextures::getSize<uint>().x, size * GridTextures::getSize<uint>().y}),
              GridTextures(cellSize.x, cellSize.y) {
        m_grid.fill(getEmptyTexture());
    }

    void setClickCell(click_handler handler) {
        clicker = handler;
    }

    void event(const sf::Event &event) override {
        if (event.type == sf::Event::MouseButtonPressed)
            mousePressed(event.mouseButton);
    }

    void updateGrid(const eCellType (&grid)[getGridSize()]) {
        for (auto i = 0; i < getGridSize(); ++i) {
            m_grid[i] = getTextureByType(grid[i]);
        }
    }

private:
    auto getTextureByType(eCellType type) {
        switch (type) {
            case eCellType::CELL_O: return getOTexture();
            case eCellType::CELL_X: return getXTexture();
            default: return getEmptyTexture();
        }
    }

    void mousePressed(const sf::Event::MouseButtonEvent& ev) {
        if (ev.button == sf::Mouse::Left) {
            auto mouse = calculateRelativePosition(ev);
            if (!clicker || !containedMouse(mouse))
                return;
            auto id = (static_cast<uint>(mouse.y) / getTextureSize().y) * size + static_cast<uint>(mouse.x) / getTextureSize().x;
            clicker(id);
        }
    }

    void draw(sf::RenderTarget& render) const override {
        for (auto line { 0 }, y { 0 }; line < m_grid.size(); line += size, y += getTextureSize().y)
            for (auto col { 0 }, x { 0 }; col < size; ++col, x += getTextureSize().x)
                render.draw(m_grid[line + col]->getSprite(x, y));
    }

    std::array<GridCell*, getGridSize()> m_grid;
    click_handler clicker {nullptr };
};
using DefaultGrid = GridRender<3>;

#endif //PROJECT_GRIDRENDER_HPP