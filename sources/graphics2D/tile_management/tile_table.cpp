#include "tile_table.hpp"

#include "graphics2D/quad_vertex.hpp"                    // for QuadVertex
#include "graphics2D/tile_management/tile_position.hpp"  // for tile::Position, tile::Position:...
#include "graphics2D/tile_management/tilemap.hpp"  // for TileMap
#include "graphics2D/tile_management/tileset.hpp"  // for Tileset
#include "tools/assertion.hpp"
#include "tools/vector.hpp"

static unsigned int get_element_number (
    math::Vector2U position, math::Vector2U size );

/* ********************************************************************
******************************* TILES *********************************
******************************************************************** */

Tiles::Tiles( std::vector< Tile > const & tiles ) : m_vector { tiles }
{
    if ( m_vector.empty() )
    {
        std::cerr << "Tiles is empty" << std::endl;
    }

    tile::Position firstPosition { m_vector[0].get_tilemap_position() };
    for ( Tile tile : m_vector )
    {
        if ( firstPosition != tile.get_tilemap_position() )
        {
            std::cerr << "Tiles haven't same position for different depth"
                      << std::endl;
        }
    }
}

/* ********************************************************************
***************************** TILE TABLE ******************************
******************************************************************** */

namespace tile
{
    Table::Table( std::vector< Tiles > tiles, tile::Size size )
      : m_matrix { tiles }, m_size { size }
    {
        if ( tiles.size() != size.tile().x * size.tile().y )
            std::cerr << "Size and Tile Table not comptatible" << std::endl;
    }

    Table::Table( std::vector< std::vector< Tiles > > tiles )
      : m_matrix {}, m_size {{ 0u, 0u}, tile::Size::Tile }
    {
        if ( vector::is_rectangle( tiles ) )
            std::cerr << "Each columns must have the same size" << std::endl;

        m_size = tile::Size {
            math::Vector2 {tiles[0].size(), tiles.size()}
             .to_u_int(),
            tile::Size::Tile
        };

        for ( std::vector< Tiles > const & subvector : tiles )
        {
            vector::append( m_matrix, subvector );
        }
    }

    std::vector< Tiles > Table::get_line( unsigned int i_line ) const
    {
        if ( i_line >= m_size.tile().y )
        {
            std::ostringstream exceptionStream {};
            exceptionStream << "Line " << i_line
                            << " out of range for matrix of size "
                            << m_size.tile();
            throw std::out_of_range { exceptionStream.str() };
        }

        return vector::extract(
            m_matrix, i_line * m_size.tile().x,
            i_line * ( m_size.tile().x + 1 ) );
    }

    std::vector< Tiles > Table::get_column( unsigned int i_column ) const
    {
        if ( i_column >= m_size.tile().x )
        {
            std::ostringstream exceptionStream {};
            exceptionStream << "Column " << i_column
                            << " out of range for matrix of size "
                            << m_size.tile();
            throw std::out_of_range { exceptionStream.str() };
        }

        std::vector< Tiles > columnVector {};

        for ( unsigned int i_line { 0u }; i_line < m_size.tile().y; ++i_line )
        {
            columnVector.push_back( this->get_element( { i_column, i_line } ) );
        }

        return columnVector;
    }

    Tiles Table::get_element( tile::Position position ) const
    {
        if ( ! position.tile().to_point().is_inside(
                 { 0u, 0u }, m_size.tile() ) )
        {
            std::ostringstream exceptionStream {};
            exceptionStream << "Point " << position.tile()
                            << " outside of matrix of size " << m_size.tile();
            throw std::out_of_range { exceptionStream.str() };
        }

        return m_matrix[get_element_number( position.tile(), m_size.tile() )];
    }

    Tiles Table::get_element( unsigned int column, unsigned int line ) const
    {
        return get_element( { column, line } );
    }
}  // namespace tile

/* ********************************************************************
****************************** FUNCTIONS ******************************
******************************************************************** */

static unsigned int get_element_number (
    math::Vector2U position, math::Vector2U size )
{
    return position.x + position.y * size.x;
}