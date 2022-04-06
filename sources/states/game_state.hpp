#pragma once

#include "entity/player.hpp"
#include "states/state.hpp"
#include "tilemap/tilemap.hpp"

class GameState final : public State
{
  public:
    GameState( Ressources const & ressources );

    void update() override;
    void render() const override;

  private:
    TileMap m_tilemap;
    Player m_player;

    sf::View m_view;

    void init_map();
    void update_map();
};
