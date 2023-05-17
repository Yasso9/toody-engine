#pragma once

#include <vector>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "maths/vector2.hpp"

class Inputs
{
    std::vector< sf::Mouse::Button > m_mouseButtonsPressed;
    std::vector< sf::Mouse::Button > m_mouseButtonsReleased;
    std::vector< sf::Keyboard::Key > m_keyPressed;
    std::vector< sf::Keyboard::Key > m_keyReleased;
    math::Vector2F                   m_mouseMovement;
    /// Horizontal and vertical scroll
    math::Vector2F                   m_mouseScroll;
    bool                             m_mouseEnteredWindow;
    bool                             m_mouseLeftWindow;
    bool                             m_focusGained;
    bool                             m_focusLost;
    bool                             m_windowClosed;
    bool                             m_windowResized;
    char                             m_textEntered;

  public:
    Inputs();

    void update ( sf::Event event );
    void clear ();

    bool           is_pressed ( sf::Mouse::Button button ) const;
    bool           is_released ( sf::Mouse::Button button ) const;
    bool           is_pressed ( sf::Keyboard::Key key ) const;
    bool           is_released ( sf::Keyboard::Key key ) const;
    math::Vector2F get_mouse_movement () const;
    math::Vector2F get_mouse_scroll () const;
    bool           is_mouse_entered () const;
    bool           is_mouse_left () const;
    bool           is_focus_gained () const;
    bool           is_focus_lost () const;
    bool           is_window_closed () const;
    bool           is_window_resized () const;
    char           get_text_entered () const;
};