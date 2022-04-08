#pragma once

#include "graphics/openGL.hpp"
#include "graphics/sfml.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "vector"

class Shape final
{
  public:
    struct Data
    {
        /// @brief Array of all the attributes of the shape
        std::vector< float > vertices;
        /// @brief Array of all the indices of the shape
        ///        If empty, we don't create an element buffer object
        std::vector< unsigned int > indices;

        /// @brief Array of the vector's size contained in each point
        std::vector< unsigned int > dataPerPoint;

        /// @brief Array size of each point
        unsigned int get_data_per_point_sum() const;
    };

    Shape();
    virtual ~Shape();

    /**
     * @brief Create the shape. If the indices is empty,
     *        we don't create an element buffer object
     */
    void create( Data const & data );

    void translate( glm::vec3 const & tranlationVector );
    void rotate( glm::vec3 const & rotationVector, float const & angle );
    void scale( glm::vec3 const & scaleVector );

    void update( glm::mat4 const & projection, glm::mat4 const & view );
    void draw() const;

  private:
    sf::Texture m_texture;
    sf::Shader m_shader;
    // Shaders m_shaders;

    /// @brief space where the object is and where we can move it
    gl::SpaceMatrix m_space;
    glm::mat4 m_spaceModel;

    unsigned int m_vertexArrayObject;
    unsigned int m_vertexBufferObject;
    /// @brief Needed to simplify the point for the vertices
    unsigned int m_elementBufferObject;

    Shape::Data m_data;

    void load_textures_and_shaders();

    /// @brief Return true if we need an element buffer object for the actual shape,
    ///        false otherwise
    bool is_element_buffer_set() const;

    void objects_generation();
    void objects_binding();
    void vertex_shader_attribution();
    void unbind();

    void transform();

    // Assign the space model to the identity matrix
    void reset_space_model();
};