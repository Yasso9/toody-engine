#pragma once

#include <project/player/player.hpp>
#include <project/states/state.hpp>
#include <project/tilemap/tilemap.hpp>

class GameState final : public State
{
  public:
    GameState( t_texturesMap const & textures, t_fontsMap const & fonts );

    void update() override;
    void render( sf::RenderWindow & target ) override;

    ~GameState() = default;

  private:
    Player m_player {};
    TileMap m_tilemap;

    sf::View m_view {};

    void init_map();

    void handle_keyboard_press( std::string const & inputName ) override;
    void handle_global_input() override;
    void update_map();
};
