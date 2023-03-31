#include "graphic.hpp"

#include <algorithm>  // for max

#include <GLM/ext/vector_float3.hpp>  // for vec3

#include "components/component.hpp"  // for Component::add_childs
#include "graphics2D/sfml.hpp"
#include "interface/window.hpp"
#include "libraries/imgui.hpp"
#include "states/input.hpp"

static Shape::S_Data SAMPLE_DATA_A {
    { // positions          // colors         // texture coords   // normals
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f },
    {},
    { 3u, 2u } };

static Shape::S_Data SAMPLE_DATA_B { {
                                         -0.5f, -0.5f,
                                         0.0f,  // bottom left
                                         0.5f, -0.5f,
                                         0.0f,  // bottom right
                                         0.0f, 0.5f,
                                         0.0f  // top center
                                     },
                                     {},
                                     { 3u, 2u } };

GraphicState::GraphicState( GameContext & gameContext )
  : State { gameContext },
    m_camera {},
    m_shapes { Shape { m_camera, SAMPLE_DATA_A } },
    // m_models { Model { m_camera, "object3D/backpack.obj" } }
    m_models {}
{
    this->add_child( m_camera );
    this->add_childs( m_shapes );
    this->add_childs( m_models );

    for ( Shape & shape : m_shapes )
    {
        shape.move( glm::vec3 { 5.f, -3.f, -1.f } );
        shape.rotate( glm::vec3 { 1.f, 1.f, 1.f }, 0.f );
        shape.scale( glm::vec3 { 1.f, 1.f, 1.f } );
    }

    for ( Model & model : m_models )
    {
        model.move( glm::vec3 { 0.f, 0.f, 0.f } );
    }
}

void GraphicState::update( UpdateContext /* context */ ) {}
