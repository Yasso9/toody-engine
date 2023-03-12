#pragma once

#include <GLM/ext/vector_float3.hpp>  // for vec3
#include <GLM/fwd.hpp>                // for mat4
#include <SFML/System/Vector2.hpp>    // for Vector2f

#include "component/component.hpp"
#include "maths/maths.hpp"

class Camera : public Component
{
  public:
    /// @todo replace that by a 3D vector with negatif or positiv values
    /// @brief Defines several possible options for camera movement.
    enum class E_Movement
    {
        Up = 0,
        Down,
        Left,
        Right,
        In,
        Out
    };

  private:
    /// @brief Position of the camera in the space
    math::Vector3F m_position;
    /// @brief Direction to where the camera is looking
    /// @todo should always be normalized -> have a set and a get
    math::Vector3F m_direction;

    math::Vector3F m_xAxis;
    math::Vector3F m_yAxis;
    math::Vector3F m_zAxis;

    float m_movementSpeed;
    float m_fieldOfView;

    bool m_captureMouse;

  public:
    Camera();

  private:
    void update ( UpdateContext context ) override;

  public:
    /// @brief Space transformation
    glm::mat4 get_projection ( float aspectRatio ) const;
    /// @brief View transformation
    glm::mat4 get_view () const;

    void set_direction ( math::Vector3F direction );

    /// @brief reset the direction and the position
    void reset ();

    void move ( Camera::E_Movement direction );
    void rotate ( math::Vector3F angleVector );

  private:
    float get_field_of_view () const;

    /// @brief return the position where the camera looks
    /// in the trigonometric circle
    math::Vector3F get_target () const;
};