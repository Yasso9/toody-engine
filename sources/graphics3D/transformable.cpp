#include "transformable.hpp"

#include <stddef.h>  // for NULL
#include <string>    // for allocator

#include <SFML/Graphics/Glsl.hpp>    // for Mat4
#include <SFML/Graphics/Shader.hpp>  // for Shader
#include <glm/fwd.hpp>               // for mat4
#include <glm/glm.hpp>               // for radians
#include <glm/gtc/type_ptr.hpp>      // for value_ptr

#include "graphics3D/camera.hpp"  // for Camera

class Render;

Transformable::Transformable( Camera const & camera, sf::Shader & shader )
  : m_camera { camera }, m_shader { shader }, m_spaceModel { 1.f }
{}

void Transformable::update_all( UpdateContext context )
{
    this->Component::update_all( context );

    // Update the shader
    gl::S_SpaceMatrix spaceMatrix {
        this->get_space_matrix( context.window.get_aspect_ratio() ) };
    m_shader.setUniform(
        "model", sf::Glsl::Mat4 { glm::value_ptr( spaceMatrix.model ) } );
    m_shader.setUniform(
        "view", sf::Glsl::Mat4 { glm::value_ptr( spaceMatrix.view ) } );
    m_shader.setUniform(
        "projection",
        sf::Glsl::Mat4 { glm::value_ptr( spaceMatrix.projection ) } );
}

void Transformable::render_all( RenderContext context ) const
{
    sf::Shader::bind( &m_shader );

    this->Component::render_all( context );

    sf::Shader::bind( NULL );
}

void Transformable::move( math::Vector3F tranlationVector )
{
    glm::mat4 const translationMatrix {
        glm::translate( glm::mat4 { 1.f }, tranlationVector.to_glm() ) };

    m_spaceModel *= translationMatrix;
}

void Transformable::rotate( math::Vector3F rotationVector, float angle )
{
    glm::mat4 const rotationMatrix { glm::rotate(
        glm::mat4 { 1.f }, glm::radians( angle ), rotationVector.to_glm() ) };

    m_spaceModel *= rotationMatrix;
}

void Transformable::scale( math::Vector3F scaleVector )
{
    glm::mat4 const scaleMatrix {
        glm::scale( glm::mat4 { 1.f }, scaleVector.to_glm() ) };
    m_spaceModel *= scaleMatrix;
}

gl::S_SpaceMatrix Transformable::get_space_matrix( float aspectRatio ) const
{
    return { m_camera.get_projection( aspectRatio ), m_camera.get_view(),
             m_spaceModel };
}

sf::Shader & Transformable::get_shader()
{
    return m_shader;
}