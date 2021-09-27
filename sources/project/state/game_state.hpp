#pragma once

#include <project/player/player.hpp>
#include <project/state/state.hpp>
#include <project/tilemap/tilemap.hpp>

class GameState final : public State
{
  public:
    GameState( std::map<TextureKey, sf::Texture> const & textures,
               std::map<FontKey, sf::Font> const & fonts,
               sf::Vector2u const & windowSize );

    void update() override;
    void render( sf::RenderWindow & target ) override;

    ~GameState() = default;

  private:
    Player m_player {};
    TileMap m_tilemap {};

    sf::View m_view {};

    void init_map();

    void handle_keyboard_press( std::string const & inputName ) override;
    void handle_global_input() override;
    void update_map();
};