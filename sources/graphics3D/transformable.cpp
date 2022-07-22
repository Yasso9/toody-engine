#include "transformable.hpp"

#include "tools/string.hpp"

Transformable::Transformable( Camera const & camera, sf::Shader & shader )
  : m_camera { camera }, m_shader { shader }, m_spaceModel { 1.f }
{}

void Transformable::update_extra( float deltaTime )
{
    gl::S_SpaceMatrix const spaceMatrix { this->get_space_matrix() };

    m_shader.setUniform(
        "model"s,
        sf::Glsl::Mat4 { glm::value_ptr( spaceMatrix.model ) } );
    m_shader.setUniform(
        "view"s,
        sf::Glsl::Mat4 { glm::value_ptr( spaceMatrix.view ) } );
    m_shader.setUniform(
        "projection"s,
        sf::Glsl::Mat4 { glm::value_ptr( spaceMatrix.projection ) } );

    this->update_custom( deltaTime );
}

void Transformable::update_custom( float /* deltaTime */ ) {}

// draws the model, and thus all its meshes
void Transformable::render( Render & render ) const
{
    sf::Shader::bind( &m_shader );

    this->render_custom( render );

    sf::Shader::bind( NULL );
}

void Transformable::render_custom( Render & /* render */ ) const {}

void Transformable::move( math::Vector3F tranlationVector )
{
    glm::mat4 const translationMatrix {
        glm::translate( glm::mat4 { 1.f }, tranlationVector.to_glm() )
    };

    m_spaceModel *= translationMatrix;
}
void Transformable::rotate( math::Vector3F rotationVector, float angle )
{
    glm::mat4 const rotationMatrix { glm::rotate( glm::mat4 { 1.f },
                                                  glm::radians( angle ),
                                                  rotationVector.to_glm() ) };

    m_spaceModel *= rotationMatrix;
}
void Transformable::scale( math::Vector3F scaleVector )
{
    glm::mat4 const scaleMatrix { glm::scale( glm::mat4 { 1.f },
                                              scaleVector.to_glm() ) };
    m_spaceModel *= scaleMatrix;
}

gl::S_SpaceMatrix Transformable::get_space_matrix() const
{
    return { m_camera.get_projection(), m_camera.get_view(), m_spaceModel };
}

sf::Shader & Transformable::get_shader()
{
    return m_shader;
}