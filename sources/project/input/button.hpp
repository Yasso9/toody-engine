#pragma once

#include <project/utility/utility.hpp>

class Button final : public sf::Drawable
{
public:
    Button();
    Button( sf::Font const & font );
    Button( sf::Font const & font, std::string const & string );

    void set_position( sf::Vector2f const & position );
    void set_position( float const & positionX, float const & positionY );
    void set_string( std::string const & string );

    void set_selected( bool const & isSelected );
    void set_pressed( bool const & isPressed );

    bool is_inside( sf::Vector2f const & mousePosition ) const;

    /* Regarde si un bouton est pressé et si oui, retourne le numero du nouveau bouton pressé, sinon cela
    retourne le même bouton, à utiliser dans les fonctions update de state lorsqu'on veut actualiser les boutons */
    unsigned int update_press( sf::Vector2f const & position, bool const & inputIsPress );
    /* Met a jour le rendu de selection si la souris passe par dessus le bouton, de même que update_press,
    à utiliser dans les fonctions updates*/
    void update_select( sf::Vector2f const & position );

private:
    sf::RectangleShape m_shape {};
    sf::Text m_text {};
    bool m_isSelected { false };
    bool m_isPressed { false };

    void adjust_text_position();

    void draw( sf::RenderTarget & target, sf::RenderStates states ) const override;
};