#pragma once

#include "3D_model/model.hpp"
#include "graphics3D/camera.hpp"
#include "graphics3D/shape.hpp"
#include "states/state.hpp"

class GraphicState final : public State
{
  public:
    GraphicState();
    virtual ~GraphicState() = default;

    void update_extra( float deltaTime ) override;
    // void render( sf::RenderTarget & target ) const override;

  private:
    Camera m_camera;

    Shape m_shape;

    Model m_modelA;
    Model m_modelB;
    Model m_modelC;

    void mouse_scroll( float const & deltaScroll ) override;
    /// @todo delete that function
    void extra_events();
};