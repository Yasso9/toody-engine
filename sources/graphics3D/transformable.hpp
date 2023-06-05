#pragma once

#include <glm/ext/matrix_float4x4.hpp>           // for mat4

#include "application/components/component.hpp"  // for Component
#include "graphics3D/openGL.hpp"                 // for S_SpaceMatrix
#include "maths/vector3.hpp"                     // for Vector3F

class Camera;
class Render;

namespace sf
{
    class Shader;
}  // namespace sf

class Transformable : public Component
{
    Camera const & m_camera;
    sf::Shader &   m_shader;

    glm::mat4 m_spaceModel;

  protected:
    Transformable( Camera const & camera, sf::Shader & shader );

  public:
    virtual ~Transformable() = default;

  private:
    virtual void update ( UpdateContext & context ) override;
    virtual void render_before ( RenderContext & context ) const override;
    virtual void render_after ( RenderContext & context ) const override;

  public:
    void move ( math::Vector3F tranlationVector );
    void rotate ( math::Vector3F rotationVector, float angle );
    void scale ( math::Vector3F scaleVector );

    gl::S_SpaceMatrix get_space_matrix ( float aspectRatio ) const;
    sf::Shader &      get_shader ();
};
