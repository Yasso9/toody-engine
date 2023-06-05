#include "camera.hpp"

#include <string>                        // for allocator

#include <SFML/Window/Keyboard.hpp>      // for Keyboard, Keyboard::B, Keyb...
#include <glm/detail/type_vec3.hpp>      // for vec<>::(anonymous), vec
#include <glm/ext/matrix_float4x4.hpp>   // for mat4
#include <glm/glm.hpp>                   // for radians
#include <glm/gtc/matrix_transform.hpp>  // for perspective

#include "application/interface/window.hpp"  // for Window
#include "imgui/imgui.hpp"
#include "maths/geometry/point.tpp"          // for Point::Point<Type>
#include "maths/vector2.hpp"                 // for Vector2F, Vector2I, Vector2U
#include "maths/vector2.tpp"    // for operator==, Vector2::operat...
#include "maths/vector3.hpp"    // for Vector3F
#include "maths/vector3.tpp"    // for Vector3::rotate
                                // for is_pressed, get_mouse_position
#include "tools/assertion.hpp"  // for ASSERTION
#include "tools/singleton.tpp"  // for Singleton::get_instance

Camera::Camera()
  : m_position {},
    m_direction {},
    m_xAxis {},
    m_yAxis {},
    m_zAxis {},
    m_movementSpeed { 0.f },
    m_fieldOfView { 45.f },
    m_captureMouse { false }
{
    this->reset();
}

void Camera::update( UpdateContext & context )
{
    m_movementSpeed = 2.5f * context.deltaTime;

    // {  // Camera configuration
    //     if ( context.inputs.is_pressed( sf::Keyboard::Space ) )
    //     {
    //         m_captureMouse = ! m_captureMouse;
    //     }
    //     if ( ImGui::Begin( "Camera" ) )
    //     {
    //         ImGui::Checkbox( "Mouse Captured ? (SPACE)", &m_captureMouse );
    //     }
    //     ImGui::End();
    // }

    // {  // Update the camera position
    //     if ( context.inputs.is_pressed( sf::Keyboard::Z ) )
    //     {
    //         this->move( Camera::E_Movement::Up );
    //     }
    //     if ( context.inputs.is_pressed( sf::Keyboard::S ) )
    //     {
    //         this->move( Camera::E_Movement::Down );
    //     }
    //     if ( context.inputs.is_pressed( sf::Keyboard::Q ) )
    //     {
    //         this->move( Camera::E_Movement::Left );
    //     }
    //     if ( context.inputs.is_pressed( sf::Keyboard::D ) )
    //     {
    //         this->move( Camera::E_Movement::Right );
    //     }
    //     if ( context.inputs.is_pressed( sf::Keyboard::A ) )
    //     {
    //         this->move( Camera::E_Movement::In );
    //     }
    //     if ( context.inputs.is_pressed( sf::Keyboard::E ) )
    //     {
    //         this->move( Camera::E_Movement::Out );
    //     }
    // }

    // {  // Update the camera direction
    //     math::Vector3F rotation { 0.f, 0.f, 0.f };

    // if ( context.inputs.is_pressed( sf::Keyboard::Up ) )
    // {
    //     rotation += { 1.f, 0.f, 0.f };
    // }
    // if ( context.inputs.is_pressed( sf::Keyboard::Down ) )
    // {
    //     rotation += { -1.f, 0.f, 0.f };
    // }
    // if ( context.inputs.is_pressed( sf::Keyboard::Left ) )
    // {
    //     rotation += { 0.f, 1.f, 0.f };
    // }
    // if ( context.inputs.is_pressed( sf::Keyboard::Right ) )
    // {
    //     rotation += { 0.f, -1.f, 0.f };
    // }
    // if ( context.inputs.is_pressed( sf::Keyboard::B ) )
    // {
    //     rotation += { 0.f, 0.f, -1.f };
    // }
    // if ( context.inputs.is_pressed( sf::Keyboard::N ) )
    // {
    //     rotation += { 0.f, 0.f, 1.f };
    // }

    // this->rotate( rotation * 20.f );
    // }

    // {  // Update the camera direction with the mouse
    //     if ( m_captureMouse )
    //     {
    //         math::Vector2F const windowCenter {
    //             context.window.get_center_position() };
    //         math::Vector2F const mousePosition {
    //             input::get_mouse_position( context.window ) };

    // if ( windowCenter == mousePosition )
    // {
    //     return;
    // }
    // // Gap between mousePosition and windowCenter
    // math::Vector2F offset { mousePosition - windowCenter };

    // // Reset Mouse Position
    // input::set_mouse_position( context.window, windowCenter.to_int() );

    // this->rotate( { offset.y, offset.x, 0.f } );
    // }
    // }

    if ( ImGui::Begin( "Camera" ) )
    {
        if ( ImGui::Button( "Reset Camera" ) )
        {
            this->reset();
        }

        // ImGui::P_InputVector3F(
        //     "Direction", m_direction, [] ( math::Vector3F & directionChanged
        //     ) {
        //         directionChanged = math::normalize( directionChanged );
        //     } );

        std::stringstream output {};
        output << "Position : " << m_position << "\n";
        output << "Direction : " << m_direction << "\n";
        output << "Normalized Direction : " << math::normalize( m_direction )
               << "\n";
        output << "X Axis : " << m_xAxis << "\n";
        output << "Y Axis : " << m_yAxis << "\n";
        output << "Z Axis : " << m_zAxis << "\n";
        output << "Movement Speed : " << m_movementSpeed << "\n";
        output << "Field Of View : " << m_fieldOfView << "\n";
        ImGui::Text( "%s", output.str().c_str() );
    }
    ImGui::End();
}

glm::mat4 Camera::get_projection( float aspectRatio ) const
{
    float const fieldOfView { glm::radians( this->get_field_of_view() ) };
    float const nearDistanceFromCamera { 0.1f };
    float const farDistanceFromCamera { 100.f };

    return glm::perspective( fieldOfView, aspectRatio, nearDistanceFromCamera,
                             farDistanceFromCamera );
}

glm::mat4 Camera::get_view() const
{
    return glm::lookAt( m_position.to_glm(), this->get_target().to_glm(),
                        { 0.f, 1.f, 0.f } );
}

void Camera::set_direction( math::Vector3F direction )
{
    m_direction = math::normalize( direction );

    m_xAxis = { 1.f, 0.f, 0.f };
    m_yAxis = { 0.f, 1.f, 0.f };
    m_zAxis = { 0.f, 0.f, 1.f };
}

void Camera::reset()
{
    this->set_direction( { 0.f, 0.f, -1.f } );
    m_position = { 0.0f, 0.0f, 5.0f };
}

void Camera::move( Camera::E_Movement direction )
{
    switch ( direction )
    {
    case Camera::E_Movement::Left :
        m_position -= m_xAxis * m_movementSpeed;
        break;
    case Camera::E_Movement::Right :
        m_position += m_xAxis * m_movementSpeed;
        break;

    case Camera::E_Movement::Up :
        m_position += m_yAxis * m_movementSpeed;
        break;
    case Camera::E_Movement::Down :
        m_position -= m_yAxis * m_movementSpeed;
        break;

    case Camera::E_Movement::In :
        m_position -= m_zAxis * m_movementSpeed;
        break;
    case Camera::E_Movement::Out :
        m_position += m_zAxis * m_movementSpeed;
        break;
    }
}

void Camera::rotate( math::Vector3F angle )
{
    angle *= m_movementSpeed;

    glm::mat3 const zMatrixRotation { math::cosinus_degree( angle.z ),
                                      -math::sinus_degree( angle.z ),
                                      0.f,
                                      math::sinus_degree( angle.z ),
                                      math::cosinus_degree( angle.z ),
                                      0.f,
                                      0.f,
                                      0.f,
                                      1.f };

    glm::mat3 const yMatrixRotation {
        math::cosinus_degree( angle.y ),
        0.f,
        math::sinus_degree( angle.y ),
        0.f,
        1.f,
        0.f,
        -math::sinus_degree( angle.y ),
        0.f,
        math::cosinus_degree( angle.y ),
    };

    glm::mat3 const xMatrixRotation {
        1.f,
        0.f,
        0.f,
        0.f,
        math::cosinus_degree( angle.x ),
        -math::sinus_degree( angle.x ),
        0.f,
        math::sinus_degree( angle.x ),
        math::cosinus_degree( angle.x ),
    };

    if ( angle.z != 0.f )
    {
        m_direction = zMatrixRotation * m_direction.to_glm();
        m_xAxis     = zMatrixRotation * m_xAxis.to_glm();
        m_yAxis     = zMatrixRotation * m_yAxis.to_glm();

        m_direction = math::normalize( m_direction );
    }
    if ( angle.x != 0.f )
    {
        m_direction = xMatrixRotation * m_direction.to_glm();
        m_yAxis     = xMatrixRotation * m_yAxis.to_glm();
        m_zAxis     = xMatrixRotation * m_zAxis.to_glm();

        m_direction = math::normalize( m_direction );
    }
    if ( angle.y != 0.f )
    {
        m_direction = yMatrixRotation * m_direction.to_glm();
        m_xAxis     = yMatrixRotation * m_xAxis.to_glm();
        m_zAxis     = yMatrixRotation * m_zAxis.to_glm();

        m_direction = math::normalize( m_direction );
    }
}

float Camera::get_field_of_view() const
{
    return m_fieldOfView;
}

math::Vector3F Camera::get_target() const
{
    return m_position + m_direction;
}
