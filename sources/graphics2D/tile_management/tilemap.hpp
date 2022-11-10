#pragma once

#include <vector>  // for vector

#include "component/transformable2D.hpp"  // for TransformableComponent
#include "graphics2D/tile_management/cursor.hpp"
#include "graphics2D/tile_management/tile.hpp"           // for Tile
#include "graphics2D/tile_management/tile_selector.hpp"  // for TileSelector
#include "maths/vector2.hpp"  // for Vector2F, Vector2U
#include "maths/vector2.tpp"  // for operator/, Vector2::oper...
#include "tools/databases.hpp"

class Render;
class Tileset;
class View;

class TileMap : public Transformable2D
{
    db::Table                                         m_databaseTable;
    tile::Selector                                    m_tileSelector;
    tile::Cursor                                      m_cursor;
    /// @brief view of the component that call the tilemap
    View &                                            m_view;
    /// @todo use TileTable
    /// @brief m_tileTable[line][column][depth]
    std::vector< std::vector< std::vector< Tile > > > m_tileTable;
    unsigned int                                      m_currentDepth;

  public:
    TileMap( View & view );
    virtual ~TileMap() = default;

    /// @brief size of the tilemap
    tile::Size get_size () const;

    math::Vector2F  get_center ( bool isAbsolutePosition = true ) const;
    Tileset const & get_tileset () const;

    /// @brief resize
    void set_tile_size ( math::Vector2U const & tileSize );

    /// @brief save the tilemap table into the database
    void save () const;

    void update_before ( float deltaTime ) override;

  private:
    void load_from_database ();

    void change_tile (
        math::Vector2U const & tilePositionInTile,
        unsigned int const &   newTileValue );

    void update_selection ();
    void update_table_informations ();
    void update_tile_size_button ();

    void render_before ( Render & render ) const override;
};