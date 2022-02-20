#pragma once

#include "graphics/openGL.hpp"

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
  public:
    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum class E_Movement
    {
        Forward = 0,
        Backward,
        Left,
        Right
    };

    // constructor with vectors
    Camera( glm::vec3 const & position = glm::vec3 { 0.0f, 0.0f, 0.0f },
            glm::vec3 const & up = glm::vec3 { 0.0f, 1.0f, 0.0f },
            float const & yaw = YAW, float const & pitch = PITCH );

    float get_zoom() const;

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 get_view_matrix() const;

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void process_keyboard( Camera::E_Movement const & direction,
                           float const & deltaTime );

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void process_mouse_movement( float & xoffset, float & yoffset,
                                 GLboolean const & constrainPitch = true );

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void process_mouse_scroll( float const & yoffset );

  private:
    // camera Attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    // euler Angles
    float m_yaw;
    float m_pitch;
    // camera options
    float m_movementSpeed;
    float m_mouseSensitivity;
    float m_zoom;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void update_camera_vectors();
};