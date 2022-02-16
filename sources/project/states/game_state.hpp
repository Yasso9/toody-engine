#pragma once

#include "entity/player.hpp"
#include "states/state.hpp"
#include "tilemap/tilemap.hpp"

class GameState final : public State
{
  public:
    GameState( std::shared_ptr<sf::RenderWindow> window,
               Ressources const & ressources, Settings const & settings );

    void update( float const & deltaTime ) override;
    void render() override;

  private:
    TileMap m_tilemap;
    Player m_player;

    sf::View m_view;

    void init_map();

    void handle_keyboard_press( std::string const & inputName ) override;
    void handle_current_input() override;

    void update_map();
};
