#pragma once

#include "entity/player.hpp"
#include "graphics2D/tilemap.hpp"
#include "states/state.hpp"

class GameState final : public State
{
  public:
    GameState();

    void update() override;
    void render() const override;

  private:
    TileMap m_tilemap;
    Player m_player;

    sf::View m_view;

    void init_map();
    void update_map();
};
