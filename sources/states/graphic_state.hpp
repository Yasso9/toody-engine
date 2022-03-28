#pragma once

#include "3D_model/model.hpp"
#include "graphics/camera.hpp"
#include "graphics/shape.hpp"
#include "states/state.hpp"

class GraphicState final : public State
{
  public:
    GraphicState( std::shared_ptr<sf::RenderWindow> window,
                  Ressources const & ressources );
    virtual ~GraphicState() = default;

    void update() override;
    void render() override;

  private:
    Camera m_camera;
    Shape m_shape;

    // sf::Shader m_shader;
    // Model m_model;

    void mouse_scroll( float const & deltaScroll ) override;
    void extra_events() override;

    void init_shape();

    void update_keyboard();
    void update_mouse();

    void update_camera();
};