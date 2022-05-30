#pragma once

#include <vector>

#include "graphics2D/sfml.hpp"
#include "graphics2D/tileset.hpp"

struct S_TileData
{
    int value;
    sf::VertexArray vertice;

    S_TileData( int const & aValue, sf::VertexArray const & aVertice )
      : value( aValue ), vertice( aVertice )
    {}
};

class TileMap : public sf::Drawable,
                public sf::Transformable
{
  public:
    explicit TileMap( Tileset const & tileset );
    virtual ~TileMap() = default;

    /// @brief size of the tilemap in pixel
    math::Vector2D get_size() const;
    /// @brief number of tile that the tilemap contain
    math::Vector2D get_tile_size() const;

    void update();

  protected:
    Tileset const m_tileset;
    /** @brief tri-dimensionnal vector containing the sprite number
     * of each tile of the tilemap */
    /// @brief m_tileTable[row][line][depth]
    std::vector< std::vector< std::vector< S_TileData > > > m_tileTable;
    unsigned int m_currentDepth;

    void set_tile_table(
        std::vector< std::vector< std::vector< int > > > const & table );

    void draw( sf::RenderTarget & target,
               sf::RenderStates states ) const override;
};

// class TileMapEditor final : public TileMap
// {
//   public:
//     TileMapEditor( sf::Texture const & texture );
//     virtual ~TileMapEditor() = default;

//     void change_tile( sf::Vector2u const tilePosition,
//                       unsigned int const tile );
//     void save() const;

//     void update();

//   private:
//     unsigned int m_currentDepth;
// };