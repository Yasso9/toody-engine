#pragma once

#include "graphics2D/sfml.hpp"
#include "tools/maths.hpp"

class Tileset
{
  public:
    /// @param position Position in pixel
    Tileset( sf::Texture const & texture, math::Vector2D const & position );
    virtual ~Tileset() = default;

    sf::Texture const & get_texture();

    math::Vector2D get_pixel_size() const;
    math::Vector2D get_tile_size() const;

    math::Vector2D get_tile_position_in_pixel(
        math::Vector2D const & pointPosition,
        bool isRelativePositon = true ) const;
    math::Vector2D get_tile_position_in_tile(
        math::Vector2D const & pointPosition,
        bool isRelativePositon = true ) const;

    int get_tile_value_from_pixel_position(
        math::Vector2D const & pointPositionInPixel,
        bool isRelativePositon = true ) const;
    int get_tile_value_from_tile_position(
        math::Vector2D const & pointPositionInTile ) const;

  private:
    sf::Texture const m_texture;
    /// @brief Absolute position of the tileset
    math::Vector2D m_position;
};