#pragma once

#include <algorithm>  // for copy
#include <optional>   // for optional
#include <vector>     // for vector

#include <SFML/Graphics/Texture.hpp>  // for Texture

#include "SFML/Graphics/Shader.hpp"      // for Shader
#include "graphics3D/transformable.hpp"  // for Transformable

class Camera;
class Render;

class Shape final : public Transformable
{
  public:
    struct S_Data
    {
        // Attributes of the shape
        std::vector< float >                         vertices {};
        // Indices of the shape
        std::optional< std::vector< unsigned int > > indices {};

        // Vector's size contained in each point
        std::vector< unsigned int > dataPerPoint {};

        std::string  get_vertex_shader () const;
        std::string  get_fragment_shader () const;
        sf::Shader & get_shader () const;

        /// @brief Array size of each point
        unsigned int get_data_per_point_sum () const;

        unsigned int get_number_of_element () const;
    };

  private:
    std::vector< sf::Texture > m_textures;

    unsigned int m_vertexArrayObject;
    unsigned int m_vertexBufferObject;
    // Used to simplify vertices
    unsigned int m_elementBufferObject;

    S_Data m_data;

  public:
    // Create the shape. If the indices is empty, we don't create an
    // element buffer object
    explicit Shape( Camera const & camera, S_Data const & data );
    virtual ~Shape();

  private:
    virtual void update ( UpdateContext context ) override;
    virtual void render ( RenderContext context ) const override;

  public:
    unsigned int   get_VAO () const;
    unsigned int & get_VAO ();
    unsigned int   get_VBO () const;
    unsigned int & get_VBO ();
    unsigned int   get_EBO () const;
    unsigned int & get_EBO ();
    S_Data const & get_data () const;

    void set_texture ( sf::Texture const & texture );

    // Return true if we need an element buffer object for the actual
    // shape, false otherwise
    bool is_EBO_handled () const;
};