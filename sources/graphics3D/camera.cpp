#include "camera.hpp"

#include <string>  // for allocator

#include <GLM/detail/type_vec3.hpp>      // for vec<>::(anonymous), vec
#include <GLM/ext/matrix_float4x4.hpp>   // for mat4
#include <GLM/glm.hpp>                   // for radians
#include <GLM/gtc/matrix_transform.hpp>  // for perspective
#include <SFML/Window/Keyboard.hpp>      // for Keyboard, Keyboard::B, Keyb...

#include "input/input.hpp"           // for is_pressed, get_mouse_position
#include "main/window.hpp"           // for Window
#include "maths/geometry/point.tpp"  // for Point::Point<Type>
#include "maths/vector2.hpp"         // for Vector2F, Vector2I, Vector2U
#include "maths/vector2.tpp"         // for operator==, Vector2::operat...
#include "maths/vector3.hpp"         // for Vector3F
#include "maths/vector3.tpp"         // for Vector3::rotate
#include "tools/assertion.hpp"       // for ASSERTION
#include "tools/singleton.tpp"       // for Singleton::get_instance

// Default camera values
// const float YAW = -90.0f;
// const float PITCH =  2.5f;
// const float SPEED = 2.5f;
// const float SENSITIVITY = 0.1f;
// const float ZOOM = 45.0f;

Camera::Camera()
  : m_position { 0.0f, 0.0f, 5.0f },
    m_direction { 0.f, 0.f, -1.f },
    m_xAxis { 1.f, 0.f, 0.f },
    m_yAxis { 0.f, 1.f, 0.f },
    m_zAxis { 0.f, 0.f, 1.f },
    m_movementSpeed { 0.f },
    m_fieldOfView { 45.f }
{}

void Camera::update_before( float deltaTime )
{
    m_movementSpeed = 2.5f * deltaTime;

    ImGui::P_Begin( "Camera", [*this] () {
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
    } );
}

float Camera::get_field_of_view() const
{
    return m_fieldOfView;
}

math::Vector3F Camera::get_target() const
{
    return m_position + m_direction;
}

glm::mat4 Camera::get_projection() const
{
    float const fieldOfView { glm::radians( this->get_field_of_view() ) };
    float const screenRatio { Window::get_instance().get_aspect_ratio() };
    float const nearDistanceFromCamera { 0.1f };
    float const farDistanceFromCamera { 100.f };

    return glm::perspective(
        fieldOfView, screenRatio, nearDistanceFromCamera,
        farDistanceFromCamera );
}

glm::mat4 Camera::get_view() const
{
    return glm::lookAt(
        m_position.to_glm(), this->get_target().to_glm(), m_yAxis.to_glm() );
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

[[maybe_unused]] static void reset_bounding ( float & angleValue )
{
    if ( angleValue > 180 )
    {
        angleValue = -180;
    }
    if ( angleValue < -180 )
    {
        angleValue = 180;
    }
}

void Camera::rotate( math::Vector3F angle )
{
    angle *= m_movementSpeed;

    glm::mat3 const zMatrix {
        math::cosinus_degree( angle.z ),
        -math::sinus_degree( angle.z ),
        0.f,
        math::sinus_degree( angle.z ),
        math::cosinus_degree( angle.z ),
        0.f,
        0.f,
        0.f,
        1.f };

    glm::mat3 const yMatrix {
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

    glm::mat3 const xMatrix {
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
        m_direction = zMatrix * m_direction.to_glm();
        m_xAxis     = zMatrix * m_xAxis.to_glm();
        m_yAxis     = zMatrix * m_yAxis.to_glm();
    }
    if ( angle.x != 0.f )
    {
        m_direction = xMatrix * m_direction.to_glm();
        m_yAxis     = xMatrix * m_yAxis.to_glm();
        m_zAxis     = xMatrix * m_zAxis.to_glm();
    }
    if ( angle.y != 0.f )
    {
        m_direction = yMatrix * m_direction.to_glm();
        m_xAxis     = yMatrix * m_xAxis.to_glm();
        m_zAxis     = yMatrix * m_zAxis.to_glm();
    }
}

void Camera::zoom( float factor )
{
    m_position += m_direction * m_movementSpeed * factor;
}