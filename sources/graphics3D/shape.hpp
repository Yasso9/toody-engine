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
        ///        If empty, we don't create an element buffer object
        std::optional< std::vector< unsigned int > > indices {};

        /// @brief Array of the vector's size contained in each point
        std::vector< unsigned int > dataPerPoint {};

        /// @todo assert que dataperpoint est cohérent et que si on le divise
        /// par vertices ça donne bien un entier

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
    /**
     * @brief Create the shape. If the indices is empty,
     *        we don't create an element buffer object
     */
    explicit Shape( S_Data const & data, Camera const & camera );
    virtual ~Shape();

  private:
    virtual void update_custom ( float deltaTime ) override final;
    virtual void render_custom ( Render & Render ) const override final;

  public:
    unsigned int   get_VAO () const;
    unsigned int & get_VAO ();
    unsigned int   get_VBO () const;
    unsigned int & get_VBO ();
    unsigned int   get_EBO () const;
    unsigned int & get_EBO ();
    S_Data const & get_data () const;

    /// @brief Return true if we need an element buffer object for the actual
    /// shape,
    ///        false otherwise
    bool is_EBO_handled () const;
};