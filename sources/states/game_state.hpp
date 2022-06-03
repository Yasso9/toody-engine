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
    sf::View m_view;
    TileMap m_tilemap;

    Player m_player;

    void init_map();
    void update_map();
};
