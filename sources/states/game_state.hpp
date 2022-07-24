#pragma once

#include "entity/player.hpp"
#include "graphics2D/tilemap.hpp"
#include "states/state.hpp"

class GameState final : public State
{
  public:
    GameState();

    void update_before( float deltaTime ) override;

  private:
    View m_view;
    TileMap m_tilemap;

    Player m_player;

    void init_map();
};
