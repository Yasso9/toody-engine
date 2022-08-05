#pragma once

#include <GLM/ext/vector_float3.hpp>  // for vec3
#include <GLM/fwd.hpp>                // for mat4
#include <SFML/System/Vector2.hpp>    // for Vector2f

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

    Camera();

    float get_field_of_view () const;

    glm::vec3 get_normalized_direction () const;
    /// @brief Return the target's position (to where the camera is) in space
    glm::vec3 get_target_position () const;

    /// @brief Space transformation
    glm::mat4 get_projection () const;
    /// @brief View transformation
    glm::mat4 get_view () const;

    /// @brief change the position to where the camera should look at
    void set_target_position ( glm::vec3 targetPosition );

    void move ( Camera::E_Movement direction, float deltaTime );
    void rotate ( glm::vec3 angle, float deltaTime );
    void rotate_from_target (
        glm::vec3 target, sf::Vector2f angle, float deltaTime );
    void zoom ( float factor, float deltaTime );

    void update_inputs ( float deltaTime );

  private:
    /// @brief Position of the camera in the space
    glm::vec3 m_position;
    /// @brief Direction to what the camera should aim
    glm::vec3 m_direction;

    /// @brief Type of the camera. If it should be used in a game or in a editor
    E_Type m_type;

    float m_movementSpeed;
    float m_fieldOfView;

    glm::vec3 get_x_axis () const;
    glm::vec3 get_y_axis () const;

    void update_keyboard_inputs_game ( float deltaTime );
    void update_mouse_inputs_game ( float deltaTime );

    void update_keyboard_inputs_editor ( float deltaTime );
    void update_mouse_inputs_editor ( float deltaTime );
};