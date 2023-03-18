#include "graphic.hpp"

#include <algorithm>  // for max

#include <GLM/ext/vector_float3.hpp>  // for vec3

#include "components/component.hpp"  // for Component::add_childs
#include "graphics2D/sfml.hpp"
#include "interface/window.hpp"
#include "libraries/imgui.hpp"
#include "states/input.hpp"

[[maybe_unused]] static Shape::S_Data get_sample_shape_data_A ();

GraphicState::GraphicState( GameContext & gameContext )
  : State { gameContext },
    m_camera {},
    m_shapes { Shape { m_camera, get_sample_shape_data_A() } },
    m_models { Model { m_camera, "backpack/backpack.obj" } }
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

static Shape::S_Data get_sample_shape_data_A ()
{
    std::vector< float > const vertices {
        // positions          // colors         // texture coords   // normals
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
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f };
    std::vector< unsigned int > const numberOfDataPerAttribute { 3u, 2u };

    return Shape::S_Data { vertices, {}, numberOfDataPerAttribute };
}