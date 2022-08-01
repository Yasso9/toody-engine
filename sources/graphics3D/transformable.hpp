#pragma once

#include <GLM/ext/matrix_float4x4.hpp> // for mat4

#include "graphics2D/component.hpp" // for Component3D
#include "graphics3D/openGL.hpp"    // for S_SpaceMatrix
#include "maths/vector3.hpp"        // for Vector3F

class Camera;
class Render;

namespace sf
{
    class Shader;
} // namespace sf

class Transformable : public Component3D
{
    Camera const & m_camera;
    sf::Shader & m_shader;

    glm::mat4 m_spaceModel;

  protected:
    Transformable( Camera const & camera, sf::Shader & shader );

  public:
    virtual ~Transformable() = default;

  private:
    virtual void update_before( float deltaTime ) override final;
    virtual void update_custom( float deltaTime );
    virtual void render_before( Render & render ) const override final;
    virtual void render_custom( Render & render ) const;

  public:
    void move( math::Vector3F tranlationVector );
    void rotate( math::Vector3F rotationVector, float angle );
    void scale( math::Vector3F scaleVector );

    gl::S_SpaceMatrix get_space_matrix() const;
    sf::Shader & get_shader();
};