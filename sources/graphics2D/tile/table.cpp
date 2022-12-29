#include "table.hpp"

#include "graphics2D/surface/quad_vertex.hpp"  // for QuadVertex
#include "graphics2D/tile/map.hpp"             // for TileMap
#include "graphics2D/tile/position.hpp"  // for tile::Position, tile::Position:...
#include "graphics2D/tile/set.hpp"       // for Tileset
#include "tools/array/vector.hpp"
#include "tools/assertion.hpp"

[[__maybe_unused__]] static unsigned int get_element_number (
    math::Vector2U position, math::Vector2U size );

namespace tile
{
    Table::Table( sf::Texture const & texture ) : m_texture { texture } {}

    tile::Size Table::get_size() const
    {
        return tile::Size { this->Array2D< Tile >::get_size(),
                            tile::Size::Tile };
    }

    void Table::set_size( tile::Size size )
    {
        this->Array2D< Tile >::set_size(
            size.tile(),
            Tile {
                math::Vector2F {0, 0},
                 math::Vector2F {0, 0}
        } );
    }

    void Table::update( float deltaTime )
    {
        for ( Tile tile : m_array2D )
        {
            tile.update_all( deltaTime );
        }
    }

    void Table::render( Render & render ) const
    {
        render.set_texture( m_texture );

        for ( Tile tile : m_array2D )
        {
            tile.render_all( render );
        }
    }

    std::istream & operator>> ( std::istream & stream, Table & table )
    {
        stream >> static_cast< Array2D< Tile > & >( table );

        tile::Position position {
            math::Vector2U {0u, 0u},
             table.m_size.x,
            tile::Position::Type::Tile
        };

        for ( unsigned int i_tile { 0u }; i_tile < table.m_array2D.size();
              ++i_tile )
        {
            table.m_array2D[i_tile].set_position( position );

            // Have a ++position instead
            position.set_value( position.value() + 1 );
        }

        return stream;
    }

    // Table::Table() : m_matrix {}, m_size { {0u, 0u}, tile::Size::Tile }
    // {}

    // Table::Table( std::vector< Tile > tiles, tile::Size size )
    //   : m_matrix { tiles }, m_size { size }
    // {
    //     if ( tiles.size() != size.tile().x * size.tile().y )
    //         std::cerr << "Size and Tile Table not comptatible" << std::endl;
    // }

    // Table::Table( std::vector< std::vector< Tile > > tiles ) : Table {}
    // {
    //     if ( tiles.empty() )
    //         std::cerr << "The table mustn't be empty" << std::endl;
    //     if ( ! vector::is_rectangle( tiles ) )
    //         std::cerr << "Each columns must have the same size" << std::endl;

    // m_size = tile::Size {
    //     math::Vector2 {tiles[0].size(), tiles.size()}
    //      .to_u_int(),
    //     tile::Size::Tile
    // };

    // for ( std::vector< Tile > const & subvector : tiles )
    // {
    //     vector::append( m_matrix, subvector );
    // }
    // }

    // tile::Size Table::get_size() const
    // {
    //     return m_size;
    // }

    // std::vector< Tile > Table::get_raw_vector() const
    // {
    //     return m_matrix;
    // }

    // Tile Table::get_element( tile::Position position ) const
    // {
    //     if ( ! position.is_inside( m_size ) )
    //     {
    //         std::cerr << "Point " << position.tile()
    //                   << " outside of matrix of size " << m_size.tile()
    //                   << std::endl;
    //     }

    // return m_matrix[get_element_number( position.tile(), m_size.tile() )];
    // }

    // Tile Table::get_element( unsigned int column, unsigned int line ) const
    // {
    //     return this->get_element( tile::Position { column, line } );
    // }

    // std::vector< Tile > Table::get_line( unsigned int line ) const
    // {
    //     if ( line >= m_size.tile().y )
    //     {
    //         std::cerr << "Line " << line << " out of range for matrix of size
    //         "
    //                   << m_size.tile() << std::endl;
    //     }

    // return vector::extract(
    //     m_matrix, line * m_size.tile().x, line * ( m_size.tile().x + 1 ) );
    // }

    // std::vector< Tile > Table::get_column( unsigned int i_column ) const
    // {
    //     if ( i_column >= m_size.tile().x )
    //     {
    //         std::cerr << "Column " << i_column
    //                   << " out of range for matrix of size " << m_size.tile()
    //                   << std::endl;
    //     }

    // std::vector< Tile > columnVector {};

    // for ( unsigned int i_line { 0u }; i_line < m_size.tile().y; ++i_line )
    // {
    //     columnVector.push_back( this->get_element( { i_column, i_line } ) );
    // }

    // return columnVector;
    // }

    // std::ostream & operator<< ( std::ostream & stream, Table const & table )
    // {
    //     return stream << table.m_matrix;
    // }

    // std::istream & operator>> ( std::istream & stream, Table & table )
    // {
    //     /// @todo have return stream >> table.m_matrix; instead
    //     stream >> table.m_matrix;
    //     /// @todo verify that all Tiles are connected to this table
    //     return stream;
    // }
}  // namespace tile

/* ********************************************************************
****************************** FUNCTIONS ******************************
******************************************************************** */

static unsigned int get_element_number ( math::Vector2U position,
                                         math::Vector2U size )
{
    return position.x + position.y * size.x;
}