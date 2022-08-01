#include "camera.hpp"

#include <string> // for allocator

#include <GLM/detail/type_vec3.hpp>     // for vec<>::(anonymous), vec
#include <GLM/ext/matrix_float4x4.hpp>  // for mat4
#include <GLM/glm.hpp>                  // for radians
#include <GLM/gtc/matrix_transform.hpp> // for perspective
#include <SFML/Window/Keyboard.hpp>     // for Keyboard, Keyboard::B, Keyb...

#include "input/input.hpp"          // for is_pressed, get_mouse_position
#include "main/window.hpp"          // for Window
#include "maths/geometry/point.tpp" // for Point::Point<Type>
#include "maths/vector2.hpp"        // for Vector2F, Vector2I, Vector2U
#include "maths/vector2.tpp"        // for operator==, Vector2::operat...
#include "maths/vector3.hpp"        // for Vector3F
#include "maths/vector3.tpp"        // for Vector3::rotate
#include "tools/assertion.hpp"      // for ASSERTION
#include "tools/singleton.tpp"      // for Singleton::get_instance

// Default camera values
// const float YAW = -90.0f;
// const float PITCH =  2.5f;
// const float SPEED = 2.5f;
// const float SENSITIVITY = 0.1f;
// const float ZOOM = 45.0f;

Camera::Camera()
  : m_position( glm::vec3 { 0.0f, 0.0f, 3.0f } ),
    m_direction(),
    m_type( E_Type::Editor ),
    m_movementSpeed(),
    m_fieldOfView( 45.f )
{
    // We start by looking at the center
    this->set_target_position( glm::vec3 { 0.f, 0.f, 0.f } );

    // Variable initialization
    this->m_movementSpeed = 2.5f;
}

float Camera::get_field_of_view() const
{
    return this->m_fieldOfView;
}

glm::vec3 Camera::get_normalized_direction() const
{
    return glm::normalize( this->m_direction );
}

glm::vec3 Camera::get_target_position() const
{
    return this->m_position + this->m_direction;
}

glm::mat4 Camera::get_projection() const
{
    float const fieldOfView { glm::radians( this->get_field_of_view() ) };
    float const screenRatio { Window::get_instance().get_aspect_ratio() };
    float const nearDistanceFromCamera { 0.1f };
    float const farDistanceFromCamera { 100.f };

    return glm::perspective( fieldOfView,
                             screenRatio,
                             nearDistanceFromCamera,
                             farDistanceFromCamera );
}

glm::mat4 Camera::get_view() const
{
    // auto view = glm::lookAt( this->m_position,
    //                          this->get_target_position(),
    //                          this->m_upAxis );
    // std::cout << "View : " << view << std::endl;

    constexpr glm::vec3 const pureYAxis { 0.0f, 1.0f, 0.0f };

    return glm::lookAt( this->m_position,
                        this->get_target_position(),
                        pureYAxis );
}

void Camera::set_target_position( glm::vec3 targetPosition )
{
    this->m_direction = glm::normalize( targetPosition - this->m_position );
}

void Camera::move( Camera::E_Movement direction, float deltaTime )
{
    float const velocity { this->m_movementSpeed * deltaTime };

    switch ( direction )
    {
    case Camera::E_Movement::Up :
        this->m_position += this->get_y_axis() * velocity;
        break;
    case Camera::E_Movement::Down :
        this->m_position -= this->get_y_axis() * velocity;
        break;
    case Camera::E_Movement::Left :
        this->m_position -= this->get_x_axis() * velocity;
        break;
    case Camera::E_Movement::Right :
        this->m_position += this->get_x_axis() * velocity;
        break;
    default :
        break;
    }
}

void Camera::rotate( glm::vec3 angle, float deltaTime )
{
    glm::vec3 const trueAngle { angle * deltaTime * 0.05f };
    // std::cout << "angle : " << angle << std::endl;
    // std::cout << "trueAngle : " << trueAngle << std::endl;

    // std::cout << "direction before" << this->m_direction << std::endl;

    math::Vector3F direction { m_direction };

    direction.rotate( trueAngle.x, math::Vector3F { 1.f, 0.f, 0.f } );
    direction.rotate( trueAngle.y, math::Vector3F { 0.f, 1.f, 0.f } );
    direction.rotate( trueAngle.z, math::Vector3F { 0.f, 0.f, 1.f } );

    this->m_direction = direction.to_glm();

    // std::cout << "direction after" << this->m_direction << std::endl;
}

void Camera::zoom( float factor, float deltaTime )
{
    float const velocity { this->m_movementSpeed * deltaTime };

    this->m_position += factor * ( this->m_direction * velocity );
}

void Camera::update_inputs( float deltaTime )
{
    switch ( this->m_type )
    {
    case E_Type::Game :
        this->update_keyboard_inputs_game( deltaTime );
        this->update_mouse_inputs_game( deltaTime );
        break;
    case E_Type::Editor :
        this->update_keyboard_inputs_editor( deltaTime );
        this->update_mouse_inputs_editor( deltaTime );
        break;
    default :
        ASSERTION( false, "Enum value unknown" );
        break;
    }
}

glm::vec3 Camera::get_x_axis() const
{
    constexpr glm::vec3 const pureYAxis { 0.0f, 1.0f, 0.0f };
    return glm::normalize( glm::cross( this->m_direction, pureYAxis ) );
}

glm::vec3 Camera::get_y_axis() const
{
    return glm::normalize(
        glm::cross( this->get_x_axis(), this->m_direction ) );
}

void Camera::update_keyboard_inputs_game( float deltaTime )
{
    if ( input::is_pressed( sf::Keyboard::Z ) )
    {
        this->move( Camera::E_Movement::Up, deltaTime );
    }
    if ( input::is_pressed( sf::Keyboard::S ) )
    {
        this->move( Camera::E_Movement::Down, deltaTime );
    }
    if ( input::is_pressed( sf::Keyboard::Q ) )
    {
        this->move( Camera::E_Movement::Left, deltaTime );
    }
    if ( input::is_pressed( sf::Keyboard::D ) )
    {
        this->move( Camera::E_Movement::Right, deltaTime );
    }

    float rotationSensivity { 20.f };

    if ( input::is_pressed( sf::Keyboard::Up ) )
    {
        this->rotate( glm::vec3 { 1.f, 0.f, 0.f } * rotationSensivity,
                      deltaTime );
    }
    if ( input::is_pressed( sf::Keyboard::Down ) )
    {
        this->rotate( glm::vec3 { -1.f, 0.f, 0.f } * rotationSensivity,
                      deltaTime );
    }
    if ( input::is_pressed( sf::Keyboard::Left ) )
    {
        this->rotate( glm::vec3 { -1.f, 0.f, 0.f } * rotationSensivity,
                      deltaTime );
    }
    if ( input::is_pressed( sf::Keyboard::Right ) )
    {
        this->rotate( glm::vec3 { 1.f, 0.f, 0.f } * rotationSensivity,
                      deltaTime );
    }
    if ( input::is_pressed( sf::Keyboard::B ) )
    {
        this->rotate( glm::vec3 { 0.f, 0.f, -1.f } * rotationSensivity,
                      deltaTime );
    }
    if ( input::is_pressed( sf::Keyboard::N ) )
    {
        this->rotate( glm::vec3 { 1.f, 0.f, 1.f } * rotationSensivity,
                      deltaTime );
    }
}

void Camera::update_mouse_inputs_game( float deltaTime )
{
    math::Vector2F const windowCenter {
        Window::get_instance().get_center_position()
    };
    math::Vector2F const currentMousePosition { input::get_mouse_position() };

    if ( windowCenter == currentMousePosition )
    {
        return;
    }

    glm::vec3 offset {};
    offset.x = currentMousePosition.x - windowCenter.x;
    offset.y = windowCenter.y - currentMousePosition.y;
    offset.z = 0.f;

    // Reset Mouse Position
    input::set_mouse_position( windowCenter.to_int() );

    float const sensitivity = 1.f;
    offset *= sensitivity;

    this->rotate( offset, deltaTime );
}

void Camera::update_keyboard_inputs_editor( float deltaTime )
{
    ( void )deltaTime;
}
void Camera::update_mouse_inputs_editor( float deltaTime )
{
    math::Vector2F const windowCenter {
        Window::get_instance().get_center_position()
    };
    math::Vector2F const currentMousePosition { input::get_mouse_position() };

    if ( windowCenter == currentMousePosition )
    {
        return;
    }

    glm::vec3 offset {};
    offset.x = currentMousePosition.x - windowCenter.x;
    offset.y = windowCenter.y - currentMousePosition.y;
    offset.z = 0.f;

    // Reset Mouse Position
    input::set_mouse_position( windowCenter.to_int() );

    float const sensitivity = 1.f;
    offset *= sensitivity;

    this->rotate( offset, deltaTime );
}