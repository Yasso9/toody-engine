#pragma once

#include <vector>

#include "graphics/openGL.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/transformable.hpp"
#include "graphics2D/sfml.hpp"

class Shape final : public Transformable
{
  public:
    struct Data
    {
        /// @brief Array of all the attributes of the shape
        std::vector< float > vertices {};
        /// @brief Array of all the indices of the shape
        ///        If empty, we don't create an element buffer object
        std::vector< unsigned int > indices {};

        /// @brief Array of the vector's size contained in each point
        std::vector< unsigned int > dataPerPoint {};

        /// @brief Array size of each point
        unsigned int get_data_per_point_sum() const;

        unsigned int get_number_of_element() const;
    };

    Shape();
    virtual ~Shape();

    /**
     * @brief Create the shape. If the indices is empty,
     *        we don't create an element buffer object
     */
    void create( Data const & data );

  private:
    sf::Texture m_textureA;
    sf::Texture m_textureB;

    unsigned int m_vertexArrayObject;
    unsigned int m_vertexBufferObject;
    /// @brief To simplify the point for the vertices
    unsigned int m_elementBufferObject;

    Shape::Data m_data;

    void update_intra() override;
    void draw_intra() const override;

    void load_textures_and_shaders();

    /// @brief Return true if we need an element buffer object for the actual shape,
    ///        false otherwise
    bool is_element_buffer_set() const;

    void objects_generation();
    void objects_binding();
    void vertex_shader_attribution();
    void unbind();
};