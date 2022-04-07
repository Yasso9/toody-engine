#pragma once

#include "graphics/openGL.hpp"
#include "graphics/sfml.hpp"

class Camera
{
  public:
    /// @brief Defines several possible options for camera movement.
    enum class E_Movement
    {
        Forward = 0,
        Backward,
        Left,
        Right
    };

    Camera();

    float get_zoom() const;

    glm::vec3 get_normalized_direction() const;
    /// @brief Return the target's position (to where the camera is) in space
    glm::vec3 get_target_position() const;

    /// @brief Space transformation
    glm::mat4 get_projection() const;
    /// @brief View transformation
    glm::mat4 get_view() const;

    /// @brief change the position to where the camera should look at
    void set_target_position( glm::vec3 const & targetPosition );

    void move( Camera::E_Movement const & direction, float const & deltaTime );

    void rotate( sf::Vector2f const & angle, float const & deltaTime );

    void zoom( float const & factor, float const & deltaTime );

    void update_inputs( float const & deltaTime );

  private:
    /// @brief Position of the camera in the space
    glm::vec3 m_position;
    /// @brief Direction to what the camera should aim
    glm::vec3 m_direction;

    /// @brief Y Axis of the camera
    glm::vec3 m_upAxis;
    /// @brief X Axis of the camera
    glm::vec3 m_rightAxis;

    float m_movementSpeed;
    float m_zoom;

    /// @brief Movement value for X axis rotation
    float m_pitch;
    /// @brief Movement value for Y axis rotation
    float m_yaw;

    /// @brief calculates the front vector from the Camera's (updated) Euler Angles
    void update_camera_vectors();

    void update_keyboard_inputs( float const & deltaTime );
    void update_mouse_inputs( float const & deltaTime );
};