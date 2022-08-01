#pragma once

#include <vector> // for vector

#include <SFML/Graphics/RectangleShape.hpp> // for RectangleShape

#include "graphics2D/component.hpp"     // for TransformableComponent2D
#include "graphics2D/tile.hpp"          // for Tile
#include "graphics2D/tile_selector.hpp" // for TileSelector
#include "maths/vector2.hpp"            // for Vector2F, Vector2U
#include "maths/vector2.tpp"            // for operator/, Vector2::oper...

class Render;
class Tileset;
class View;

class TileMap : public TransformableComponent2D
{
    TileSelector m_tileSelector;
    sf::RectangleShape m_cursor;
    /// @brief view of the component that call the tilemap
    View & m_view;
    /// @todo think of a better way to structur this variable : https://stackoverflow.com/questions/2286991/c-two-dimensional-stdvector-best-practices
    /// @brief m_tileTable[line][column][depth]
    std::vector< std::vector< std::vector< Tile > > > m_tileTable;
    unsigned int m_currentDepth;

  public:
    TileMap( View & view );
    virtual ~TileMap() = default;

    /// @brief size of the tilemap in pixel
    math::Vector2F get_size() const;
    /// @brief number of tile that the tilemap contain
    math::Vector2U get_tile_size() const;

    math::Vector2F get_center( bool isAbsolutePosition = true ) const
    {
        math::Vector2F centerPosition { this->get_size() / 2.f };
        if ( isAbsolutePosition )
        {
            centerPosition += math::Vector2F { this->getPosition() };
        }

        return centerPosition;
    }

    Tileset const & get_tileset() const
    {
        return this->m_tileSelector.get_tileset();
    };

    /// @brief resize
    void set_tile_size( math::Vector2U const & tileSize );

    /// @brief save the tilemap table into the sqlite3 database
    void save() const;

    void update_before( float deltaTime ) override;

  private:
    void init_tile_table_from_database();

    void change_tile( math::Vector2U const & tilePositionInTile,
                      int const & newTileValue );

    void update_selection();
    void update_table_informations();
    void update_tile_size_button();

    void render_before( Render & render ) const override;
};