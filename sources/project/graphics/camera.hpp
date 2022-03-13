#pragma once

#include "graphics/openGL.hpp"

/// @brief An abstract camera class that processes input and calculates
///        the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
  public:
    /// @brief Defines several possible options for camera movement.
    ///        Used as abstraction to stay away from window-system specific input methods
    enum class E_Movement
    {
        Forward = 0,
        Backward,
        Left,
        Right
    };

    /// @brief constructor with vectors
    Camera();

    float get_zoom() const;

    glm::vec3 get_normalized_direction() const;
    /// @brief Return the target's position in space
    glm::vec3 get_target_position() const;
    /// @brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 get_view_matrix() const;

    /// @brief Processes input received from any keyboard-like input system.
    ///        Accepts input parameter in the form of camera defined ENUM
    ///        (to abstract it from windowing systems)
    void move( Camera::E_Movement const & direction, float const & deltaTime );

    void rotate( glm::vec3 const & angle, float const & deltaTime );

    void zoom( float const & factor, float const & deltaTime );

  private:
    /// @brief Position of the camera in the space
    glm::vec3 m_position;
    /// @brief Direction to what the camera should aim
    glm::vec3 m_direction;

    /// @brief Y Axis of the camera
    glm::vec3 m_upAxis;
    /// @brief X Axis of the camera
    glm::vec3 m_rightAxis;

    glm::vec3 const m_pureYAxis;

    float m_movementSpeed;
    float m_zoom;

    /// @brief Movement value for X axis rotation
    float m_pitch;
    /// @brief Movement value for Y axis rotation
    float m_yaw;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void update_camera_vectors();
};