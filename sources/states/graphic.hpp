#pragma once

#include <vector>  // for vector

#include "graphics3D/camera.hpp"       // for Camera
#include "graphics3D/model/model.hpp"  // for Model
#include "graphics3D/shape.hpp"        // for Shape
#include "states/state.hpp"            // for State

class GraphicState final : public State
{
    Camera m_camera;

    std::vector< Shape > m_shapes;
    std::vector< Model > m_models;

  public:
    GraphicState( GameContext & gameContext );

    void update ( UpdateContext context ) override;
};