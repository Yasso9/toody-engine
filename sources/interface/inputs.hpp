#pragma once

#include <vector>  // for vector

#include <SFML/Window/Event.hpp>     // for sf::Event
#include <SFML/Window/Keyboard.hpp>  // for sf::Keyboard
#include <SFML/Window/Mouse.hpp>     // for sf::Mouse

#include "interface/keyboard_move.hpp"  // for
#include "interface/window.hpp"         // for Window
#include "maths/vector2.hpp"            // for Vector2I

class View;

// TODO set it as a component because of the update function
class Inputs
{
    // For set/get_mouse_position
    Window & m_window;

    std::vector< sf::Mouse::Button > m_mouseButtonsPressed;
    std::vector< sf::Mouse::Button > m_mouseButtonsReleased;
    std::vector< sf::Keyboard::Key > m_keyPressed;
    std::vector< sf::Keyboard::Key > m_keyReleased;
    math::Vector2I                   m_mouseMovement;
    /// x : Horizontal and y : vertical scroll
    math::Vector2I                   m_mouseScroll;
    bool                             m_mouseEnteredWindow;
    bool                             m_mouseLeftWindow;
    bool                             m_focusGained;
    bool                             m_focusLost;
    bool                             m_windowClosed;
    bool                             m_windowResized;
    char                             m_textEntered;

  public:
    Inputs( Window & window );

    void update ( sf::Event event );
    void clear ();

    bool           is_pressed ( sf::Mouse::Button button ) const;
    bool           is_released ( sf::Mouse::Button button ) const;
    bool           is_pressed ( sf::Keyboard::Key key ) const;
    bool           is_released ( sf::Keyboard::Key key ) const;
    math::Vector2I get_mouse_movement () const;
    math::Vector2I get_mouse_scroll () const;
    bool           is_mouse_entered () const;
    bool           is_mouse_left () const;
    bool           is_focus_gained () const;
    bool           is_focus_lost () const;
    bool           is_window_closed () const;
    bool           is_window_resized () const;
    char           get_text_entered () const;

    math::Vector2I get_mouse_position () const;
    // Mouse position relative to view
    math::Vector2F get_mouse_position ( View const & view ) const;
    void           set_mouse_position ( math::Vector2I newMousePosition ) const;

    math::Vector2F get_movement ( KeyboardMove movementKey,
                                  bool         invertMovement = false ) const;
};
