#pragma once

#include "graphics/shader.hpp"
#include "tools/sfml.hpp"
#include "vector"

class Shape final
{
  public:
    Shape() = default;
    virtual ~Shape();

    /// @brief Create the shape. If the indices is empty,
    ///        we don't create an element buffer object
    void create( std::vector<float> const & vertices,
                 std::vector<unsigned int> const & indices,
                 unsigned int const & numberOfDataPerAttribute );
    void update( gl::SpaceMatrix const & space );
    void draw() const;

  private:
    Shaders m_shaders;

    gl::SpaceMatrix m_space;

    unsigned int m_vertexArrayObject;
    unsigned int m_vertexBufferObject;
    /// @brief Needed to simplify the point for the vertices
    unsigned int m_elementBufferObject;

    /// @brief Array of all the attributes of the shape
    std::vector<float> m_vertices;
    /// @brief Array of all the indices of the shape
    ///        If empty, we don't create an element buffer object
    std::vector<unsigned int> m_indices;

    /// @brief number of value per point of each triangle of the shapes
    unsigned int m_numberOfDataPerAttribute;

    /// @brief Return true if we need an element buffer object for the actual shape,
    ///        false otherwise
    bool is_element_buffer_set() const;

    void set_vertices( std::vector<float> const & vertices );
    void set_indices( std::vector<unsigned int> const & indices );

    void objects_generation();
    void bind_objects();
    void vertex_shader_attribution();
    void unbind();

    void transform() const;
};