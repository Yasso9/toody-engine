#pragma once

#include "graphics2D/sfml.hpp"
#include "maths/maths.hpp"

class Tileset
{
  public:
    /// @param position Position in pixel
    explicit Tileset( sf::Texture const & texture,
                      math::Vector2F const & position = math::Vector2F() );
    virtual ~Tileset() = default;

    sf::Texture const & get_texture() const;
    math::Vector2F get_position() const;
    math::Vector2F get_end_position() const;

    math::Vector2U get_size_in_pixel() const;
    math::Vector2U get_size_in_tile() const;

    math::Vector2F get_tile_position_in_pixel(
        math::Vector2F const & pointPosition,
        bool isRelativePositon = true ) const;
    math::Vector2F get_tile_position_in_tile(
        math::Vector2F const & pointPosition,
        bool isRelativePositon = true ) const;

    int get_tile_value_from_pixel_position(
        math::Vector2F const & pointPositionInPixel,
        bool isRelativePositon = true ) const;
    int get_tile_value_from_tile_position(
        math::Vector2U const & pointPositionInTile ) const;

    void set_position( math::Vector2F const & position );

    bool contain( math::Vector2F const & point ) const;

  private:
    sf::Texture const m_texture;
    /// @brief Absolute position of the tileset
    math::Vector2F m_position;
};