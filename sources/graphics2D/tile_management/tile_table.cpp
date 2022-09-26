#include "tile_table.hpp"

#include "tools/assertion.hpp"
#include "tools/vector.hpp"

static unsigned int get_element_number (
    math::Vector2U position, math::Vector2U size );

/* ********************************************************************
******************************* TILES *********************************
******************************************************************** */

Tiles::Tiles( std::vector< Tile > const & tiles ) : vector { tiles } {}

/* ********************************************************************
***************************** TILE TABLE ******************************
******************************************************************** */

TileTable::TileTable( std::vector< Tiles > tiles, math::Vector2U size )
  : m_matrix { tiles }, m_size { size }
{
    ASSERTION(
        tiles.size() == size.x * size.y,
        "Size and Tile Table not comptatible" );
}

TileTable::TileTable(
    std::vector< std::vector< Tiles > > tileDimensionnalVector )
  : m_matrix {}, m_size { 0, 0 }
{
    /// @todo Assert all tiles have the same size
    m_size = {
        tileDimensionnalVector.size(), tileDimensionnalVector[0].size() };

    for ( std::vector< Tiles > const & subvector : tileDimensionnalVector )
    {
        vector::append( m_matrix, subvector );
    }
}

std::vector< Tiles > TileTable::get_line( unsigned int i_line ) const
{
    if ( i_line >= m_size.y )
    {
        std::ostringstream exceptionStream {};
        exceptionStream << "Line " << i_line
                        << " out of range for matrix of size " << m_size;
        throw std::out_of_range { exceptionStream.str() };
    }

    return vector::extract(
        m_matrix, i_line * m_size.x, i_line * ( m_size.x + 1 ) );
}

std::vector< Tiles > TileTable::get_column( unsigned int i_column ) const
{
    if ( i_column >= m_size.x )
    {
        std::ostringstream exceptionStream {};
        exceptionStream << "Column " << i_column
                        << " out of range for matrix of size " << m_size;
        throw std::out_of_range { exceptionStream.str() };
    }

    std::vector< Tiles > columnVector {};

    for ( unsigned int i_line { 0u }; i_line < m_size.y; ++i_line )
    {
        columnVector.push_back( this->get_element( { i_column, i_line } ) );
    }

    return columnVector;
}

Tiles TileTable::get_element( math::Vector2U position ) const
{
    if ( ! position.to_point().is_inside( { 0u, 0u }, position ) )
    {
        std::ostringstream exceptionStream {};
        exceptionStream << "Point " << position << " outside of matrix of size "
                        << m_size;
        throw std::out_of_range { exceptionStream.str() };
    }

    return m_matrix[get_element_number( position, m_size )];
}

Tiles TileTable::get_element( unsigned int column, unsigned int line ) const
{
    return get_element( { column, line } );
}

/* ********************************************************************
****************************** FUNCTIONS ******************************
******************************************************************** */

static unsigned int get_element_number (
    math::Vector2U position, math::Vector2U size )
{
    return position.x + position.y * size.x;
}