#pragma once

#include <GLM/ext/vector_float3.hpp>  // for vec3
#include <GLM/fwd.hpp>                // for mat4
#include <SFML/System/Vector2.hpp>    // for Vector2f

#include "graphics2D/component.hpp"
#include "maths/maths.hpp"

class Camera : public Component3D
{
  public:
    /// @brief Defines several possible options for camera movement.
    enum class E_Movement
    {
        Up = 0,
        Down,
        Left,
        Right
    };

  private:
    /// @brief X axis rotation angle
    float m_pitch;
    /// @brief Y axis rotation angle
    float m_yaw;
    /// @brief Z axis rotation angle
    float m_roll;

    /// @brief Position of the camera in the space
    math::Vector3F m_position;

    float m_movementSpeed;
    float m_fieldOfView;

  public:
    Camera();

  private:
    void update_before ( float deltaTime ) override;

  public:
    float get_field_of_view () const;

    math::Vector3F get_direction () const;
    /// @brief return the position where the camera looks
    /// in the trigonometric circle
    // math::Vector3F get_target () const;

    /// @brief Space transformation
    glm::mat4 get_projection () const;
    /// @brief View transformation
    glm::mat4 get_view () const;

    /// @brief set the position where camera should look at
    void set_target ( math::Vector3F targetPosition );

    void move ( Camera::E_Movement direction );
    void rotate ( math::Vector3F angleVector );
    void zoom ( float factor );

  private:
    glm::vec3 get_x_axis () const;
    glm::vec3 get_y_axis () const;
};