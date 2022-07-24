#include "graphic_state.hpp"

#include <cmath>
#include <iostream>

#include "tools/path.hpp"
#include "tools/tools.hpp"

[[maybe_unused]] static Shape::S_Data get_sample_shape_data_A();

GraphicState::GraphicState()
  : State( State::E_List::Graphics ),
    m_camera {
},
    m_shapes { Shape { get_sample_shape_data_A(), m_camera } },
    m_models { /* Model { "backpack/backpack.obj"s, m_camera }  */ }
{
    this->add_childs( m_shapes );
    this->add_childs( m_models );

    for ( Model & model : m_models )
    {
        model.move( glm::vec3 { 3.f, 3.f, 0.f } );
    }

    for ( Shape & shape : m_shapes )
    {
        shape.move( glm::vec3 { 3.f, -2.f, -1.f } );
        shape.rotate( glm::vec3 { 1.f, 1.f, 1.f }, 0.f );
        shape.scale( glm::vec3 { 1.f, 1.f, 1.f } );
    }
}

void GraphicState::update_before( float deltaTime )
{
    m_camera.update_inputs( deltaTime );
}

void GraphicState::mouse_scroll( float const & deltaScroll )
{
    float scrollSpeed { 5.f };

    if ( deltaScroll < 0.f )
    {
        scrollSpeed = -scrollSpeed;
    }
    // else positiv scroll
    // m_camera.zoom( scrollSpeed, m_deltaTime );
}

static Shape::S_Data get_sample_shape_data_A()
{
    std::vector< float > const vertices {
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
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f
    };
    std::vector< unsigned int > const numberOfDataPerAttribute { 3u, 2u };

    return Shape::S_Data { vertices, {}, numberOfDataPerAttribute };
}