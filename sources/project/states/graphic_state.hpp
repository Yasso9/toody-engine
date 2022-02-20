#pragma once

#include "graphics/camera.hpp"
#include "graphics/shape.hpp"
#include "states/state.hpp"

class GraphicState final : public State
{
  public:
    GraphicState( std::shared_ptr<sf::RenderWindow> window,
                  Ressources const & ressources, Settings const & settings );
    virtual ~GraphicState() = default;

    void update( float const & deltaTime ) override;
    void render() override;

  private:
    Camera m_camera;
    Shape m_shape;

    T_KeyboardInputMap init_keyboard_action() const override;
    T_MouseInputMap init_mouse_action() const override;

    void init_shape();

    void handle_current_input() override;
};