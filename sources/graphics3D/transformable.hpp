#pragma once

#include <filesystem>

#include "graphics2D/component.hpp"
#include "graphics3D/camera.hpp"
#include "graphics3D/openGL.hpp"
#include "maths/maths.hpp"

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
    virtual void update_extra( float deltaTime ) override final;
    virtual void update_custom( float deltaTime );
    virtual void render( Render & render ) const override final;
    virtual void render_custom( Window const & window ) const;

  public:
    void move( math::Vector3F tranlationVector );
    void rotate( math::Vector3F rotationVector, float angle );
    void scale( math::Vector3F scaleVector );

    gl::S_SpaceMatrix get_space_matrix() const;
    sf::Shader & get_shader();
};