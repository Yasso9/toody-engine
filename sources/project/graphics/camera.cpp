#include "camera.hpp"

Camera::Camera( glm::vec3 const & position, glm::vec3 const & up,
                float const & yaw, float const & pitch )
  : m_front( glm::vec3( 0.0f, 0.0f, -1.0f ) ),
    m_movementSpeed( SPEED ),
    m_mouseSensitivity( SENSITIVITY ),
    m_zoom( ZOOM )
{
    m_position = position;
    m_worldUp = up;
    m_yaw = yaw;
    m_pitch = pitch;

    update_camera_vectors();
}

float Camera::get_zoom() const
{
    return this->m_zoom;
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt( m_position, m_position + m_front, m_up );
}

void Camera::process_keyboard( Camera::E_Movement const & direction,
                               float const & deltaTime )
{
    float velocity = m_movementSpeed * deltaTime;
    if ( direction == Camera::E_Movement::Forward )
        m_position += m_front * velocity;
    if ( direction == Camera::E_Movement::Backward )
        m_position -= m_front * velocity;
    if ( direction == Camera::E_Movement::Left )
        m_position -= m_right * velocity;
    if ( direction == Camera::E_Movement::Right )
        m_position += m_right * velocity;
}

void Camera::process_mouse_movement( float & xoffset, float & yoffset,
                                     GLboolean const & constrainPitch )
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if ( constrainPitch )
    {
        if ( m_pitch > 89.0f )
            m_pitch = 89.0f;
        if ( m_pitch < -89.0f )
            m_pitch = -89.0f;
    }

    // update m_front, m_right and m_up Vectors using the updated Euler angles
    update_camera_vectors();
}

void Camera::process_mouse_scroll( float const & yoffset )
{
    m_zoom -= yoffset;
    if ( m_zoom < 1.0f )
        m_zoom = 1.0f;
    if ( m_zoom > 45.0f )
        m_zoom = 45.0f;
}

void Camera::update_camera_vectors()
{
    // calculate the new m_front vector
    glm::vec3 front {};

    front.x =
        std::cos( glm::radians( m_yaw ) ) * std::cos( glm::radians( m_pitch ) );
    front.y = std::sin( glm::radians( m_pitch ) );
    front.z =
        std::sin( glm::radians( m_yaw ) ) * std::cos( glm::radians( m_pitch ) );

    m_front = glm::normalize( front );

    // also re-calculate the m_right and m_up vector
    m_right = glm::normalize( glm::cross(
        m_front,
        m_worldUp ) ); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize( glm::cross( m_right, m_front ) );
}