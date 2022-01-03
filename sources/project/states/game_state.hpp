#pragma once

#include <project/entity/player.hpp>
#include <project/states/state.hpp>
#include <project/tilemap/tilemap.hpp>

class GameState final : public State
{
  public:
    GameState( Ressources const & ressources, Settings const & settings );

    void update() override;
    void render( sf::RenderWindow & target ) override;

  private:
    TileMap m_tilemap;
    Player m_player;

    sf::View m_view {};

    void init_map();

    void handle_keyboard_press( std::string const & inputName ) override;
    void handle_current_input() override;

    void update_map();
};
