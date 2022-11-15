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
    /// @todo use TileTable instead
    /// @brief m_tileTable[line][column][depth]
    std::vector< std::vector< std::vector< Tile > > > m_tileTable;

    database::Table m_databaseTable;
    tile::Selector  m_tileSelector;

    /// @todo set it as a component
    tile::Cursor m_cursor;
    /// @brief view of the component that call the tilemap
    View &       m_view;

    /// @todo maybe rework that
    unsigned int m_currentDepth;

  public:
    explicit TileMap( View & view );
    virtual ~TileMap() = default;

    void update ( float deltaTime ) override;

  private:
    void render ( Render & render ) const override;

  public:
    /// @brief Reference to the tileset that the tilemap use
    Tileset const & get_tileset () const;
    /// @brief Size of the tilemap
    tile::Size      get_size () const;
    /// @brief Absolute position of the center of the tilemap in pixel
    math::Vector2F  get_center_absolute () const;
    /// @brief Relative position of the center of the tilemap in pixel
    math::Vector2F  get_center_relative () const;

    /// @brief Get the tile position of a point in the tilemap
    /// @return optionnal tile position if the point is in the tilemap
    std::optional< tile::Position > get_tile_position (
        math::PointF point ) const;

    bool contain ( math::PointF point ) const;

    /// @brief Change the size of the tilemap
    /// @param tileSize New size of the tilemap
    void resize ( tile::Size tileSize );
    /// @brief save the tilemap table into the database
    void save () const;

  private:
    void load_from_database ();

    void change_tile (
        tile::Position tilemapPosition, tile::Position tilesetPosition );

    void update_cursor ();
    void update_tile_size_button ();
    void update_debug ();
    void update_table_debug ();
};