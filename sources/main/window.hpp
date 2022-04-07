#pragma once

#include "graphics/sfml.hpp"
#include "tools/singleton.hpp"

class Window final : public sf::RenderWindow,
                     public Singleton< Window >
{
    // We must have the constuctor so the singleton can call it
    friend Window & Singleton< Window >::get_instance();

  public:
    virtual ~Window() = default;

    sf::Vector2f get_size_f() const;
    sf::Vector2i get_size_i() const;

    float get_aspect_ratio() const;

    sf::Vector2u get_center_position_u() const;
    sf::Vector2f get_center_position_f() const;

    /// @brief Check if the window have focus and if the mouse is inside the windows
    bool has_absolute_focus() const;

  private:
    Window();

    void create();
    void initialize();
};