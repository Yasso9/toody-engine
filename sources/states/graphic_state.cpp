#include "graphic_state.hpp"

#include <cmath>
#include <iostream>

#include "tools/tools.hpp"

static Shape::Data get_sample_shape_data_A();

GraphicState::GraphicState() : State( State::E_List::Graphics )
// ,m_model( tools::get_path::resources( "backpack/backpack.obj"s ) )
{
    // this->m_shader.loadFromFile( tools::get_path::shaders( "shader.vert"s ),
    //                              tools::get_path::shaders( "shader.frag"s ) );
    // glEnable( GL_DEPTH_TEST );
    this->m_shape.create( get_sample_shape_data_A() );
}

void GraphicState::update()
{
    this->m_shape.translate( glm::vec3 { 0.f, 0.f, 0.f } );
    this->m_shape.rotate( glm::vec3 { 1.f, 1.f, 1.f }, 0.f );
    this->m_shape.scale( glm::vec3 { 1.f, 1.f, 1.f } );

    this->m_shape.update( this->m_camera.get_projection(),
                          this->m_camera.get_view() );
}

void GraphicState::render() const
{
    // sf::Shader::bind( &this->m_shader );
    // this->m_model.draw( this->m_shader );
    // sf::Shader::bind( NULL );

    this->m_shape.draw();
}

void GraphicState::mouse_scroll( float const & deltaScroll )
{
    float scrollSpeed { 5.f };

    if ( deltaScroll < 0.f )
    {
        scrollSpeed = -scrollSpeed;
    }
    else // positiv scroll
    {}

    this->m_camera.zoom( scrollSpeed, this->m_deltaTime );
}

void GraphicState::extra_events()
{
    this->m_camera.update_inputs( this->m_deltaTime );
}

static Shape::Data get_sample_shape_data_A()
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

    return Shape::Data { vertices, {}, numberOfDataPerAttribute };
}