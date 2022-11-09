#include "graphic_state.hpp"

#include <algorithm>  // for max

#include <GLM/ext/vector_float3.hpp>  // for vec3

#include "graphics2D/component.hpp"  // for Component::add_childs
#include "graphics2D/sfml.hpp"
#include "input/input.hpp"
#include "libraries/imgui.hpp"
#include "main/window.hpp"

[[maybe_unused]] static Shape::S_Data get_sample_shape_data_A ();

GraphicState::GraphicState()
  : State { State::E_List::Graphics },
    m_camera {},
    m_shapes { Shape { get_sample_shape_data_A(), m_camera } },
    m_models { Model { "backpack/backpack.obj", m_camera } },
    m_captureMouse { false }
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

void GraphicState::update_before( float /* deltaTime */ )
{
    if ( input::is_pressed( sf::Keyboard::Space ) )
    {
        m_captureMouse = ! m_captureMouse;
    }

    ImGui::P_Show( "Graphic State", [this] () {
        ImGui::Checkbox( "Mouse Captured ? (SPACE)", &m_captureMouse );
    } );

    this->update_camera_keyboard_inputs();
    this->update_camera_mouse_inputs();
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

void GraphicState::update_camera_keyboard_inputs()
{
    if ( input::is_pressed( sf::Keyboard::Z ) )
    {
        m_camera.move( Camera::E_Movement::Up );
    }
    if ( input::is_pressed( sf::Keyboard::S ) )
    {
        m_camera.move( Camera::E_Movement::Down );
    }
    if ( input::is_pressed( sf::Keyboard::Q ) )
    {
        m_camera.move( Camera::E_Movement::Left );
    }
    if ( input::is_pressed( sf::Keyboard::D ) )
    {
        m_camera.move( Camera::E_Movement::Right );
    }
    if ( input::is_pressed( sf::Keyboard::A ) )
    {
        m_camera.move( Camera::E_Movement::In );
    }
    if ( input::is_pressed( sf::Keyboard::E ) )
    {
        m_camera.move( Camera::E_Movement::Out );
    }

    math::Vector3F rotation { 0.f, 0.f, 0.f };

    if ( input::is_pressed( sf::Keyboard::Up ) )
    {
        rotation += { 1.f, 0.f, 0.f };
    }
    if ( input::is_pressed( sf::Keyboard::Down ) )
    {
        rotation += { -1.f, 0.f, 0.f };
    }
    if ( input::is_pressed( sf::Keyboard::Left ) )
    {
        rotation += { 0.f, 1.f, 0.f };
    }
    if ( input::is_pressed( sf::Keyboard::Right ) )
    {
        rotation += { 0.f, -1.f, 0.f };
    }
    if ( input::is_pressed( sf::Keyboard::B ) )
    {
        rotation += { 0.f, 0.f, -1.f };
    }
    if ( input::is_pressed( sf::Keyboard::N ) )
    {
        rotation += { 0.f, 0.f, 1.f };
    }

    m_camera.rotate( rotation * 20.f );
}

void GraphicState::update_camera_mouse_inputs()
{
    if ( ! m_captureMouse )
    {
        return;
    }

    math::Vector2F const windowCenter {
        Window::get_instance().get_center_position() };
    math::Vector2F const mousePosition { input::get_mouse_position() };

    if ( windowCenter == mousePosition )
    {
        return;
    }
    // Gap between mousePosition and windowCenter
    math::Vector2F offset { mousePosition - windowCenter };

    // Reset Mouse Position
    input::set_mouse_position( windowCenter.to_int() );

    m_camera.rotate( { offset.y, offset.x, 0.f } );
}

static Shape::S_Data get_sample_shape_data_A ()
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
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f };
    std::vector< unsigned int > const numberOfDataPerAttribute { 3u, 2u };

    return Shape::S_Data { vertices, {}, numberOfDataPerAttribute };
}