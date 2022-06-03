#pragma once

#include "graphics/openGL.hpp"

class Transformable
{
  public:
    Transformable( std::string const & vertexShaderPath,
                   std::string const & fragmentShaderPath );
    virtual ~Transformable() = default;

    void translate( glm::vec3 const & tranlationVector );
    void rotate( glm::vec3 const & rotationVector, float const & angle );
    void scale( glm::vec3 const & scaleVector );

    void update( glm::mat4 const & projection, glm::mat4 const & view );
    void draw() const;

  protected:
    sf::Shader m_shader;
    /// @brief space where the object is and where we can move it
    gl::SpaceMatrix m_space;
    glm::mat4 m_spaceModel;

    virtual void update_intra()     = 0;
    virtual void draw_intra() const = 0;

    void transform();
    /// @brief Assign the space model to the identity matrix
    void reset_space_model();
};