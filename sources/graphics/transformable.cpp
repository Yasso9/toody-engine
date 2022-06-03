#include "transformable.hpp"

#include "tools/string.hpp"

Transformable::Transformable( std::string const & vertexShaderPath,
                              std::string const & fragmentShaderPath )
  : m_shader(), m_space(), m_spaceModel()
{
    if ( ! this->m_shader.loadFromFile( vertexShaderPath, fragmentShaderPath ) )
    {
        throw std::runtime_error { "Loading error with at one shader : '"s
                                   + vertexShaderPath + "' or '"s
                                   + fragmentShaderPath + '\'' };
    }
}

void Transformable::translate( glm::vec3 const & tranlationVector )
{
    glm::mat4 const translationMatrix { glm::translate( glm::mat4 { 1.f },
                                                        tranlationVector ) };

    this->m_spaceModel *= translationMatrix;
}
void Transformable::rotate( glm::vec3 const & rotationVector,
                            float const & angle )
{
    glm::mat4 const rotationMatrix { glm::rotate(
        glm::mat4 { 1.f },
        glm::radians( angle ),
        glm::vec3 { rotationVector } ) };

    this->m_spaceModel *= rotationMatrix;
}
void Transformable::scale( glm::vec3 const & scaleVector )
{
    glm::mat4 const scaleMatrix { glm::scale( glm::mat4 { 1.f },
                                              glm::vec3 { scaleVector } ) };
    this->m_spaceModel *= scaleMatrix;
}

void Transformable::update( glm::mat4 const & projection,
                            glm::mat4 const & view )
{
    this->m_space.projection = projection;
    this->m_space.view       = view;
    this->m_space.model      = this->m_spaceModel;

    // All the object transformation have been made, so we reset the matrix to identity
    this->reset_space_model();

    this->transform();

    this->update_intra();
}

// draws the model, and thus all its meshes
void Transformable::draw() const
{
    sf::Shader::bind( &this->m_shader );

    this->draw_intra();

    sf::Shader::bind( NULL );
}

void Transformable::transform()
{
    this->m_shader.setUniform(
        "model"s,
        sf::Glsl::Mat4 { glm::value_ptr( this->m_space.model ) } );
    this->m_shader.setUniform(
        "view"s,
        sf::Glsl::Mat4 { glm::value_ptr( this->m_space.view ) } );
    this->m_shader.setUniform(
        "projection"s,
        sf::Glsl::Mat4 { glm::value_ptr( this->m_space.projection ) } );
}

void Transformable::reset_space_model()
{
    this->m_spaceModel = glm::mat4 { 1.f };
}