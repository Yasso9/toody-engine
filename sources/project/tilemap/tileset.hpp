#pragma once

#include "output_info/information.hpp"

class Tileset final : public sf::Drawable, public sf::Transformable
{
  public:
    Tileset( sf::Texture const & texture );

    Tileset( const Tileset & ) = delete;
    Tileset( Tileset && ) = delete;
    Tileset & operator=( const Tileset & ) = delete;
    Tileset & operator=( Tileset && ) = delete;

    virtual ~Tileset() = default;

    sf::Vector2f get_size() const;
    void set_size( sf::Vector2f const & size );
    void set_size( float const & sizeX, float const & sizeY );

    int get_selected_tile() const;

    /** @brief If the tilemap should be print, set it to false,
     * otherwise set it to true */
    void switch_print();

    void update( sf::Vector2f const & mousePosition,
                 bool const & buttonIsPressed );

    virtual void draw( sf::RenderTarget & target,
                       sf::RenderStates states ) const override;

  private:
    sf::Sprite const m_image;
    Cursor m_cursor;

    /// @brief Number of the tile selected
    int m_tileSelected;

    bool m_isPrint { false };

    void update_cursor( sf::Vector2f const & cursorPosition );

    /** @brief Check if the position is in the tileset */
    bool include( sf::Vector2f const & position ) const;
    /** @brief Convert a given position to the tilemap coordinate  */
    sf::Vector2u get_tile_position( sf::Vector2f const & position ) const;
};