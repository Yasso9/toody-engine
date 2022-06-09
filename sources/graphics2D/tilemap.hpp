#pragma once

#include <vector>

#include "graphics2D/sfml.hpp"
#include "graphics2D/tile_selector.hpp"
#include "graphics2D/tileset.hpp"

/// @todo améliorer cette class et la rendre plus éprouvé en delete toute les choses lié à vertex array
struct TileQuad
{
  public:
    TileQuad();
    virtual ~TileQuad() = default;

    sf::VertexArray const & get_vertex_array() const;

    void set_position( math::Vector2D const & tilemapPosition,
                       math::Vector2D const & tileCoordinate );

    void set_texture_coordinate( int const & tileValue,
                                 unsigned int numberOfXAxisTile );

    sf::Vertex operator[]( size_t index ) const;

  private:
    sf::VertexArray m_vextexArray;
};

struct S_TileData
{
    int value {};
    TileQuad quad {};

    S_TileData( int const & aValue, TileQuad const & aVertice )
      : value( aValue ), quad( aVertice )
    {}
};

class TileMap : public sf::Drawable,
                public sf::Transformable
{
  public:
    TileMap( sf::View & view );
    virtual ~TileMap() = default;

    /// @brief size of the tilemap in pixel
    math::Vector2D get_size() const;
    /// @brief number of tile that the tilemap contain
    math::Vector2D get_tile_size() const;

    void update();

  protected:
    TileSelector m_tileSelector;
    sf::RectangleShape m_cursor;
    sf::View & m_view;
    /** @brief tri-dimensionnal vector containing the sprite number
     * of each tile of the tilemap */
    /// @brief m_tileTable[row][line][depth]
    std::vector< std::vector< std::vector< S_TileData > > > m_tileTable;
    unsigned int m_currentDepth;

    void set_tile_table(
        std::vector< std::vector< std::vector< int > > > const & table );

    void change_tile( int const & newTileValue,
                      math::Vector2D const & tilePositionInTile );

    void draw( sf::RenderTarget & target,
               sf::RenderStates states ) const override;
};