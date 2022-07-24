#pragma once

#include <vector>

#include "3D_model/model.hpp"
#include "graphics3D/camera.hpp"
#include "graphics3D/shape.hpp"
#include "states/state.hpp"

class GraphicState final : public State
{
    Camera m_camera;

    std::vector< Shape > m_shapes;
    std::vector< Model > m_models;

  public:
    GraphicState();

    void update_before( float deltaTime ) override;

  private:
    void mouse_scroll( float const & deltaScroll ) override;
};