#pragma once

#include <cstddef>  // for size_t

#include <SFML/Graphics/Texture.hpp>  // for Texture

#include "maths/geometry/point.hpp"  // for PointF, PointU
#include "maths/vector2.hpp"         // for Vector2F, Vector2U

class Tileset
{
    sf::Texture const m_texture;
    /// @brief Absolute position of the tileset
    math::Vector2F    m_position;

  public:
    /// @param position Position in pixel
    explicit Tileset(
        sf::Texture const &    texture,
        math::Vector2F const & position = math::Vector2F() );
    virtual ~Tileset() = default;

    sf::Texture const & get_texture () const;
    math::Vector2F      get_position () const;
    math::Vector2F      get_end_position () const;

    math::Vector2U get_size_in_pixel () const;
    math::Vector2U get_size_in_tile () const;
    std::size_t    get_number_of_tile () const;

    math::Vector2F get_tile_position_in_pixel (
        math::Vector2F const & pointPosition,
        bool                   isRelativePositon = true ) const;
    math::Vector2F get_tile_position_in_tile (
        math::Vector2F const & pointPosition,
        bool                   isRelativePositon = true ) const;

    int get_tile_value_from_pixel_position (
        math::Vector2F const & pointPositionInPixel,
        bool                   isRelativePositon = true ) const;
    int get_tile_value_from_tile_position (
        math::PointU const & pointPositionInTile ) const;

    void set_position ( math::Vector2F const & position );

    bool contain ( math::PointF const & point ) const;
};