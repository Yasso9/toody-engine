#include "graphic_state.hpp"

#include <cmath>
#include <iostream>

#include "tools/path.hpp"
#include "tools/tools.hpp"

[[maybe_unused]] static Shape::Data get_sample_shape_data_A();

GraphicState::GraphicState()
  : State( State::E_List::Graphics ),
    m_camera( Camera::E_Type::Game ),
    m_shape(),
    // m_model( "C:/Users/Turki/Downloads/CAR/AMGGT.obj" )
    // m_model( "C:/Users/Turki/Downloads/GUN/acp smith.obj" )
    // m_model( "C:/Users/Turki/Downloads/BED/black_bed.obj" )
    m_modelA( "C:/Users/Turki/Documents/Model3D/house.obj"s ),
    m_modelB( path::get_folder( path::E_Folder::Resources )
              / "backpack/backpack.obj"s ),
    m_modelC( "C:/Users/Turki/Documents/Model3D/door.obj"s )
{
    this->m_shape.create( get_sample_shape_data_A() );
}

void GraphicState::update()
{
    this->m_modelA.update( this->m_camera.get_projection(),
                           this->m_camera.get_view() );

    this->m_modelB.translate( glm::vec3 { 3.f, 3.f, 0.f } );
    this->m_modelB.update( this->m_camera.get_projection(),
                           this->m_camera.get_view() );

    this->m_modelC.translate( glm::vec3 { 0.f, -2.f, 0.f } );
    this->m_modelC.update( this->m_camera.get_projection(),
                           this->m_camera.get_view() );

    this->m_shape.translate( glm::vec3 { 3.f, -2.f, -1.f } );
    this->m_shape.rotate( glm::vec3 { 1.f, 1.f, 1.f }, 0.f );
    this->m_shape.scale( glm::vec3 { 1.f, 1.f, 1.f } );
    this->m_shape.update( this->m_camera.get_projection(),
                          this->m_camera.get_view() );
}

void GraphicState::render() const
{
    this->m_modelA.draw();
    this->m_modelB.draw();
    this->m_modelC.draw();

    this->m_shape.draw();
}

void GraphicState::mouse_scroll( float const & deltaScroll )
{
    float scrollSpeed { 5.f };

    if ( deltaScroll < 0.f )
    {
        scrollSpeed = -scrollSpeed;
    }
    // else positiv scroll

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