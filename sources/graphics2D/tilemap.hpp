#pragma once

#include <vector>

#include "graphics2D/sfml.hpp"
#include "graphics2D/tile.hpp"
#include "graphics2D/tile_selector.hpp"
#include "graphics2D/tileset.hpp"

class TileMap : public sf::Drawable,
                public sf::Transformable
{
  public:
    TileMap( sf::View & view );
    virtual ~TileMap() = default;

    /// @brief size of the tilemap in pixel
    math::Vector2F get_size() const;
    /// @brief number of tile that the tilemap contain
    math::Vector2U get_tile_size() const;

    /// @brief resize
    void set_tile_size( math::Vector2U const & tileSize );

    void process_events();
    void update();

    void save() const;

  protected:
    TileSelector m_tileSelector;
    sf::RectangleShape m_cursor;
    sf::View & m_view;
    /** @brief tri-dimensionnal vector containing the sprite number
     * of each tile of the tilemap */
    /// @brief m_tileTable[line][column][depth]
    std::vector< std::vector< std::vector< Tile > > > m_tileTable;
    unsigned int m_currentDepth;

    bool m_isLeftButtonPressed;
    math::Vector2F m_mousePosition;

    void set_tile_table(
        std::vector< std::vector< std::vector< int > > > const & table );

    void change_tile( math::Vector2U const & tilePositionInTile,
                      int const & newTileValue );

    void draw( sf::RenderTarget & target,
               sf::RenderStates states ) const override;
};