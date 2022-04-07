#include "camera.hpp"

#include <cmath>
#include <iostream>

#include "main/window.hpp"

// Default camera values
// const float YAW = -90.0f;
// const float PITCH =  2.5f;
// const float SPEED = 2.5f;
// const float SENSITIVITY = 0.1f;
// const float ZOOM = 45.0f;

Camera::Camera()
{
    // Initialize the position of the camera
    this->m_position = glm::vec3 { 0.0f, 0.0f, 3.0f };
    // We start by looking at the center
    this->set_target_position( glm::vec3 { 0.f, 0.f, 0.f } );

    // Variable initialization
    this->m_zoom          = 45.f;
    this->m_movementSpeed = 2.5f;
    this->m_pitch         = 2.5f;
    this->m_yaw           = -90.f;

    this->update_camera_vectors();
}

float Camera::get_zoom() const
{
    return this->m_zoom;
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
    float const fieldOfView { glm::radians( this->get_zoom() ) };
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
    return glm::lookAt( this->m_position,
                        this->get_target_position(),
                        this->m_upAxis );
}

void Camera::set_target_position( glm::vec3 const & targetPosition )
{
    this->m_direction = targetPosition - this->m_position;
}

void Camera::move( Camera::E_Movement const & direction,
                   float const & deltaTime )
{
    float const velocity { this->m_movementSpeed * deltaTime };

    switch ( direction )
    {
    case Camera::E_Movement::Forward :
        this->m_position += this->m_upAxis * velocity;
        // this->m_target += this->m_upAxis * velocity;
        break;
    case Camera::E_Movement::Backward :
        this->m_position -= this->m_upAxis * velocity;
        // this->m_target -= this->m_upAxis * velocity;
        break;
    case Camera::E_Movement::Left :
        this->m_position -= this->m_rightAxis * velocity;
        // this->m_target -= this->m_rightAxis * velocity;
        break;
    case Camera::E_Movement::Right :
        this->m_position += this->m_rightAxis * velocity;
        // this->m_target += this->m_rightAxis * velocity;
        break;
    default :
        break;
    }

    this->update_camera_vectors();
}

void Camera::rotate( sf::Vector2f const & angle, float const & deltaTime )
{
    this->m_yaw += angle.x * deltaTime;
    this->m_pitch += angle.y * deltaTime;

    if ( this->m_pitch > 89.0f )
    {
        this->m_pitch = 89.0f;
    }
    if ( this->m_pitch < -89.0f )
    {
        this->m_pitch = -89.0f;
    }

    this->update_camera_vectors();
}

void Camera::zoom( float const & factor, float const & deltaTime )
{
    float const velocity { this->m_movementSpeed * deltaTime };

    this->m_position += factor * ( this->m_direction * velocity );

    this->update_camera_vectors();
}

void Camera::update_inputs( float const & deltaTime )
{
    this->update_keyboard_inputs( deltaTime );
    this->update_mouse_inputs( deltaTime );
}

void Camera::update_camera_vectors()
{
    this->m_direction.x = std::cos( glm::radians( this->m_pitch ) )
                          * std::cos( glm::radians( this->m_yaw ) );
    this->m_direction.y = std::sin( glm::radians( this->m_pitch ) );
    this->m_direction.z = std::cos( glm::radians( this->m_pitch ) )
                          * std::sin( glm::radians( this->m_yaw ) );

    this->m_direction = glm::normalize( this->m_direction );

    constexpr glm::vec3 const pureYAxis { 0.0f, 1.0f, 0.0f };

    // Update the up and right axis
    this->m_rightAxis =
        glm::normalize( glm::cross( this->m_direction, pureYAxis ) );
    this->m_upAxis =
        glm::normalize( glm::cross( this->m_rightAxis, this->m_direction ) );

    // std::cout << "Right Axis : " << this->m_rightAxis << std::endl;
    // std::cout << "Up Axis : " << this->m_upAxis << std::endl;
}

void Camera::update_keyboard_inputs( float const & deltaTime )
{
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )
    {
        this->move( Camera::E_Movement::Forward, deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
    {
        this->move( Camera::E_Movement::Backward, deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) )
    {
        this->move( Camera::E_Movement::Left, deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
    {
        this->move( Camera::E_Movement::Right, deltaTime );
    }

    float const & rotationSensivity { 20.f };

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) )
    {
        this->rotate( sf::Vector2f { 0.f, 1.f } * rotationSensivity,
                      deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) )
    {
        this->rotate( sf::Vector2f { 0.f, -1.f } * rotationSensivity,
                      deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) )
    {
        this->rotate( sf::Vector2f { -1.f, 0.f } * rotationSensivity,
                      deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) )
    {
        this->rotate( sf::Vector2f { 1.f, 0.f } * rotationSensivity,
                      deltaTime );
    }
}

void Camera::update_mouse_inputs( float const & deltaTime )
{
    sf::Vector2f const windowCenter {
        Window::get_instance().get_center_position_f()
    };
    sf::Vector2f const currentMousePosition { sf::Mouse::getPosition(
        Window::get_instance() ) };

    sf::Vector2f offset {};
    offset.x = currentMousePosition.x - windowCenter.x;
    offset.y = windowCenter.y - currentMousePosition.y;

    // Reset Mouse Position
    // sf::Mouse::setPosition( static_cast< sf::Vector2i >( windowCenter ),
    //                         Window::get_instance() );

    float const sensitivity = 1.f;
    offset *= sensitivity;

    this->rotate( offset, deltaTime );
}