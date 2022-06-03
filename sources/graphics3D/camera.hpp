#pragma once

#include "graphics2D/sfml.hpp"
#include "graphics3D/openGL.hpp"

class Camera
{
  public:
    /// @brief Change the possibilities that the camera can have
    enum class E_Type
    {
        Game = 0,
        Editor,
    };

    /// @brief Defines several possible options for camera movement.
    enum class E_Movement
    {
        Up = 0,
        Down,
        Left,
        Right
    };

    Camera( E_Type const & type );

    float get_field_of_view() const;

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
    void rotate( glm::vec3 const & angle, float const & deltaTime );
    void rotate_from_target( glm::vec3 const & target,
                             sf::Vector2f const & angle,
                             float const & deltaTime );
    void zoom( float const & factor, float const & deltaTime );

    void update_inputs( float const & deltaTime );

  private:
    /// @brief Position of the camera in the space
    glm::vec3 m_position;
    /// @brief Direction to what the camera should aim
    glm::vec3 m_direction;

    /// @brief Type of the camera. If it should be used in a game or in a editor
    E_Type m_type;

    float m_movementSpeed;
    float m_fieldOfView;

    glm::vec3 get_x_axis() const;
    glm::vec3 get_y_axis() const;

    void update_keyboard_inputs_game( float const & deltaTime );
    void update_mouse_inputs_game( float const & deltaTime );

    void update_keyboard_inputs_editor( float const & deltaTime );
    void update_mouse_inputs_editor( float const & deltaTime );
};