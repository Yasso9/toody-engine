#pragma once

#include <vector>  // for vector

#include "3D_model/model.hpp"     // for Model
#include "graphics3D/camera.hpp"  // for Camera
#include "graphics3D/shape.hpp"   // for Shape
#include "states/state.hpp"       // for State

class GraphicState final : public State
{
    Camera m_camera;

    std::vector< Shape > m_shapes;
    std::vector< Model > m_models;

  public:
    GraphicState();

    void update_before ( float deltaTime ) override;

  private:
    void mouse_scroll ( float const & deltaScroll ) override;

    void update_camera_keyboard_inputs ();
    void update_camera_mouse_inputs ();
};