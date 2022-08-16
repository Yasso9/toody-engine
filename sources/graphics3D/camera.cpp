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
  : m_pitch { -90.0f },
    m_yaw { 2.5f },
    m_roll { 0.f },
    m_position { 0.0f, 0.0f, 3.0f },
    m_movementSpeed { 2.5f },
    m_fieldOfView { 45.f }
{
    this->set_target( { 0.f, 0.f, 0.f } );
}

void Camera::update_before( float /* deltaTime */ )
{
    ImGui::P_Begin( "Camera", [*this] () {
        std::stringstream output {};
        output << "Position : " << m_position << "\n";
        output << "Direction : " << this->get_direction() << "\n";
        output << "Pitch : " << m_pitch << "\n";
        output << "Yaw : " << m_yaw << "\n";
        output << "Roll : " << m_roll << "\n";
        ImGui::Text( "%s", output.str().c_str() );
    } );
}

float Camera::get_field_of_view() const
{
    return m_fieldOfView;
}

math::Vector3F Camera::get_direction() const
{
    math::Vector3F direction { 0.f, 0.f, 0.f };

    direction.y += math::cosinus_degree( m_pitch );
    direction.z += math::sinus_degree( m_pitch );

    direction.z += math::cosinus_degree( m_yaw );
    direction.x += math::sinus_degree( m_yaw );

    direction.x += math::cosinus_degree( m_roll );
    direction.y += math::sinus_degree( m_roll );

    return direction;
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
    math::Vector3F const pureYAxis { 0.0f, 1.0f, 0.0f };

    return glm::lookAt(
        m_position.to_glm(), this->get_direction().to_glm(),
        pureYAxis.to_glm() );
}

void Camera::set_target( math::Vector3F /* targetPosition */ )
{
    // math::Vector3F direction = targetPosition - m_position;
}

void Camera::move( Camera::E_Movement direction )
{
    switch ( direction )
    {
    case Camera::E_Movement::Up :
        m_position += this->get_y_axis() * m_movementSpeed;
        break;
    case Camera::E_Movement::Down :
        m_position -= this->get_y_axis() * m_movementSpeed;
        break;
    case Camera::E_Movement::Left :
        m_position -= this->get_x_axis() * m_movementSpeed;
        break;
    case Camera::E_Movement::Right :
        m_position += this->get_x_axis() * m_movementSpeed;
        break;
    }
}

static void reset_bounding ( float & angleValue )
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
    angle *= 0.05f;

    m_pitch += angle.x;
    m_yaw += angle.y;
    m_roll += angle.z;

    reset_bounding( m_pitch );
    reset_bounding( m_yaw );
    reset_bounding( m_roll );
}

void Camera::zoom( float factor )
{
    m_position += this->get_direction() * m_movementSpeed * factor;
}

glm::vec3 Camera::get_x_axis() const
{
    constexpr glm::vec3 const pureYAxis { 0.0f, 1.0f, 0.0f };
    return glm::normalize(
        glm::cross( this->get_direction().to_glm(), pureYAxis ) );
}

glm::vec3 Camera::get_y_axis() const
{
    return glm::normalize(
        glm::cross( this->get_x_axis(), this->get_direction().to_glm() ) );
}