#pragma once

#include <project/ressources-handling/texture_sprite.hpp>
#include <project/engine/entity.hpp>

class Player
{
public:
    Player();

    sf::Vector2f get_position() const;

    void create( sf::Texture const & texture );

    void stay();
    void walk( Direction const & dir );
    void run();

    void update();
    void render( sf::RenderWindow & target );

    ~Player();
private:
    TextureSprite m_textureSprite {};

    sf::Texture m_texture {};
    sf::Sprite m_sprite {};

    PlayerState m_state {};
    Direction m_direction {};

    std::string m_name {};

    float m_speed {};
};