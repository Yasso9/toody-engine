#pragma once

#include <vector>

#include "graphics2D/sfml.hpp"

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
    TileMap( sf::Texture const & texture );
    virtual ~TileMap() = default;

    /// @brief size of the tilemap in pixel
    sf::Vector2f get_size() const;

  protected:
    /// @brief texture used to draw the tilemap
    sf::Texture const m_texture;
    /** @brief tri-dimensionnal vector containing the sprite number
     * of each tile of the tilemap */
    /// @brief m_tileTable[row][line][depth]
    std::vector< std::vector< std::vector< S_TileData > > > m_tileTable;

    void set_tile_table(
        std::vector< std::vector< std::vector< int > > > const & table );

    void draw( sf::RenderTarget & target,
               sf::RenderStates states ) const override;
};

class TileMapEditor final : public TileMap
{
  public:
    TileMapEditor( sf::Texture const & texture );
    virtual ~TileMapEditor() = default;

    void change_tile( sf::Vector2u const tilePosition,
                      unsigned int const tile );
    void save() const;

    void update();

  private:
    unsigned int m_currentDepth;
};