#include "camera.hpp"

#include <cmath>
#include <iostream>

// Default camera values
// const float YAW = -90.0f;
// const float PITCH =  2.5f;
// const float SPEED = 2.5f;
// const float SENSITIVITY = 0.1f;
// const float ZOOM = 45.0f;

Camera::Camera() : m_pureYAxis( 0.0f, 1.0f, 0.0f )
{
    // Initialize the position of the camera
    this->m_position = glm::vec3 { 0.0f, 0.0f, 3.0f };
    // Initialize the direction to look at the center
    this->m_direction = glm::vec3 { 0.f } - this->m_position;

    this->m_zoom = 45.f;
    this->m_movementSpeed = 2.5f;

    this->m_pitch = 2.5f;
    this->m_yaw = -90.f;

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

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt( this->m_position,
                        this->get_target_position(),
                        this->m_upAxis );
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
    // std::cout << "\n" << std::endl;

    this->m_yaw += angle.x * deltaTime;
    this->m_pitch += angle.y * deltaTime;

    // if ( pitch > 89.0f )
    //     pitch = 89.0f;
    // if ( pitch < -89.0f )
    //     pitch = -89.0f;

    // std::cout << "Yaw : " << this->m_yaw << std::endl;
    // std::cout << "Pitch : " << this->m_pitch << std::endl;

    this->update_camera_vectors();
}

void Camera::zoom( float const & factor, float const & deltaTime )
{
    float const velocity { this->m_movementSpeed * deltaTime };

    this->m_position += factor * ( this->m_direction * velocity );

    this->update_camera_vectors();
}

// void Camera::process_mouse_movement( float & /* xoffset */,
//                                      float & /* yoffset */,
//                                      GLboolean const & /* constrainPitch */ )
// {
// xoffset *= m_mouseSensitivity;
// yoffset *= m_mouseSensitivity;

// m_yaw += xoffset;
// m_pitch += yoffset;

// // make sure that when pitch is out of bounds, screen doesn't get flipped
// if ( constrainPitch )
// {
//     if ( m_pitch > 89.0f )
//         m_pitch = 89.0f;
//     if ( m_pitch < -89.0f )
//         m_pitch = -89.0f;
// }

// // update m_front, m_rightAxis and m_upAxis Vectors using the updated Euler angles
// update_camera_vectors();
// }

void Camera::update_camera_vectors()
{
    // std::cout << "Direction Before : " << this->m_direction << std::endl;

    this->m_direction.x = std::cos( glm::radians( this->m_pitch ) )
                          * std::cos( glm::radians( this->m_yaw ) );
    this->m_direction.y = std::sin( glm::radians( this->m_pitch ) );
    this->m_direction.z = std::cos( glm::radians( this->m_pitch ) )
                          * std::sin( glm::radians( this->m_yaw ) );

    this->m_direction = glm::normalize( this->m_direction );

    // std::cout << "Direction After : " << this->m_direction << std::endl;

    // Update the up and right axis
    this->m_rightAxis =
        glm::normalize( glm::cross( this->m_direction, this->m_pureYAxis ) );
    this->m_upAxis =
        glm::normalize( glm::cross( this->m_rightAxis, this->m_direction ) );

    // std::cout << "Right Axis : " << this->m_rightAxis << std::endl;
    // std::cout << "Up Axis : " << this->m_upAxis << std::endl;
}

// TYPO à garder
// static float get_relativ_length( glm::vec3 const & vector3 )
// {
//     glm::vec3 factor { 1.f, 1.f, 1.f };
//     if ( vector3.x < 0.f )
//     {
//         factor.x = -factor.x;
//     }
//     else if ( vector3.y < 0.f )
//     {
//         factor.y = -factor.y;
//     }
//     else if ( vector3.z < 0.f )
//     {
//         factor.z = -factor.z;
//     }

//     float squareLength { factor.x * std::pow( vector3.x, 2.f )
//                          + factor.y * std::pow( vector3.y, 2.f )
//                          + factor.z * std::pow( vector3.z, 2.f ) };

//     bool isLengthNegative { false };
//     if ( squareLength < 0.f )
//     {
//         isLengthNegative = true;
//         squareLength = -squareLength;
//     }

//     float length { std::sqrt( squareLength ) };
//     if ( isLengthNegative )
//     {
//         length = -length;
//     }

//     return length;
// }