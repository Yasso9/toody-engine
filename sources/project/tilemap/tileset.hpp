#pragma once

#include <project/output-info/information.hpp>

class Tileset final : public sf::Drawable, public sf::Transformable
{
  public:
    Tileset( sf::Texture const & texture );

    Tileset( const Tileset & ) noexcept = delete;
    Tileset( Tileset && ) noexcept = delete;
    Tileset & operator=( const Tileset & ) = delete;
    Tileset & operator=( Tileset && ) noexcept = delete;

    sf::Vector2f get_size() const noexcept;
    void set_size( sf::Vector2f const & size ) noexcept;
    void set_size( float const & sizeX, float const & sizeY ) noexcept;

    /** @brief If the tilemap should be print, set it to false,
     * otherwise set it to true */
    void switch_print();

    void update( sf::Vector2f const & mousePosition, unsigned int & tile,
                 bool const & buttonIsPressed );

    virtual void draw( sf::RenderTarget & target,
                       sf::RenderStates states ) const override;

    virtual ~Tileset() noexcept = default;

  private:
    sf::Sprite const m_image;
    Cursor m_cursor {};

    bool m_isPrint { false };

    void update_cursor( sf::Vector2f const & cursorPosition );

    /** @brief Check if the position is in the tileset */
    bool include( sf::Vector2f const & position ) const;
    /** @brief Convert a given position to the tilemap coordinate  */
    sf::Vector2u get_tile_position( sf::Vector2f const & position ) const;
};