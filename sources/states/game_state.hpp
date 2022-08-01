#pragma once

#include "entity/player.hpp"      // for Player
#include "graphics2D/tilemap.hpp" // for TileMap
#include "graphics2D/view.hpp"    // for View
#include "states/state.hpp"       // for State

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
