#pragma once

#include <project/utility/utility.hpp>

// Faire un template de message en mode sdtvector
class Message final
{
public:
    Message();

    void create( sf::Font const & font, unsigned int const textNumber );
    void update( sf::Vector2f const position, std::vector<std::string> const & message = {} );
    void render( sf::RenderWindow & target ) const;

private:
    // A transformer en array peut etre
    std::vector<sf::Text> m_textArray {};
};



class TileCursor : public sf::RectangleShape
{
public:
    TileCursor();

    void update( sf::Vector2f const cursorPosition, sf::Vector2f const mapBeginPosition );
};