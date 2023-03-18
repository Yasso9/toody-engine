#pragma once

#include <algorithm>  // for copy
#include <optional>   // for optional
#include <vector>     // for vector

#include <SFML/Graphics/Texture.hpp>  // for Texture

#include "graphics3D/transformable.hpp"  // for Transformable

class Camera;
class Render;

class Shape final : public Transformable
{
  public:
    struct S_Data
    {
        /// @brief Array of all the attributes of the shape
        std::vector< float >                         vertices {};
        /// @brief Array of all the indices of the shape
        std::optional< std::vector< unsigned int > > indices {};

        /// @brief Array of the vector's size contained in each point
        std::vector< unsigned int > dataPerPoint {};

        /// @brief Array size of each point
        unsigned int get_data_per_point_sum () const;

        unsigned int get_number_of_element () const;
    };

  private:
    std::vector< sf::Texture > m_textures;

    unsigned int m_vertexArrayObject;
    unsigned int m_vertexBufferObject;
    /// @brief To simplify the point for the vertices
    unsigned int m_elementBufferObject;

    S_Data m_data;

  public:
    /// @brief Create the shape. If the indices is empty, we don't create an
    /// element buffer object
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

    /// @brief Return true if we need an element buffer object for the actual
    /// shape, false otherwise
    bool is_EBO_handled () const;
};