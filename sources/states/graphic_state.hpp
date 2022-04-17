#pragma once

#include "3D_model/model.hpp"
#include "graphics/camera.hpp"
#include "graphics/shape.hpp"
#include "states/state.hpp"

class GraphicState final : public State
{
  public:
    GraphicState();
    virtual ~GraphicState() = default;

    void update() override;
    void render() const override;

  private:
    Camera m_camera;

    Shape m_shape;

    Model m_modelA;
    Model m_modelB;
    Model m_modelC;

    void mouse_scroll( float const & deltaScroll ) override;
    void extra_events() override;
};