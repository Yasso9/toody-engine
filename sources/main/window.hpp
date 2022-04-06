#pragma once

#include "tools/sfml.hpp"
#include "tools/singleton.hpp"

class Window final : public sf::RenderWindow,
                     public Singleton< Window >
{
    // We must have the constuctor so the singleton can call it
    friend Window & Singleton< Window >::get_instance();

  public:
    virtual ~Window() = default;

  private:
    Window();

    void create();
    void initialize();
};