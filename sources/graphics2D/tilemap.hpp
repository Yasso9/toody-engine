#pragma once

#include <vector>

#include "graphics2D/sfml.hpp"
#include "graphics2D/tile.hpp"
#include "graphics2D/tile_selector.hpp"
#include "graphics2D/tileset.hpp"

class TileMap : public sf::Drawable,
                public sf::Transformable
{
    TileSelector m_tileSelector;
    sf::RectangleShape m_cursor;
    /// @todo check if the reference is useful here by comparing the argument reference ande the member ref
    sf::View & m_view;
    /// @todo think of a better way to structur this variable : https://stackoverflow.com/questions/2286991/c-two-dimensional-stdvector-best-practices
    /// @brief m_tileTable[line][column][depth]
    std::vector< std::vector< std::vector< Tile > > > m_tileTable;
    unsigned int m_currentDepth;

    bool m_isLeftButtonPressed;
    math::Vector2I m_mousePosition;

  public:
    TileMap( sf::View & view );
    virtual ~TileMap() = default;

    /// @brief size of the tilemap in pixel
    math::Vector2F get_size() const;
    /// @brief number of tile that the tilemap contain
    math::Vector2S get_tile_size() const;

    /// @brief resize
    void set_tile_size( math::Vector2S const & tileSize );

    void process_events();
    void update();

    /// @brief save the tilemap table into the sqlite3 database
    void save() const;

  private:
    void init_tile_table_from_database();

    void change_tile( math::Vector2U const & tilePositionInTile,
                      int const & newTileValue );

    void update_selection();
    void update_table_informations();
    void update_tile_size_button();

    void draw( sf::RenderTarget & target,
               sf::RenderStates states ) const override;
};