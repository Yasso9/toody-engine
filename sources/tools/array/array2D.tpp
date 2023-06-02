#pragma once

#include "array2D.hpp"

#include "tools/array/vector.hpp"  // for vector::append, vector::is_rectangle
#include "tools/traces.hpp"        // for Trace

template< typename T >
Array2D< T >::Array2D() : m_array2D {}, m_size { 0u, 0u }
{}

template< typename T >
Array2D< T >::Array2D( std::vector< T > const & array, math::Vector2U size )
  : m_array2D { array }, m_size { size }
{
    if ( array.size() != size.x * size.y )
    {
        Trace::Warning( "Size and Array not comptatible" );
    }
}

template< typename T >
Array2D< T >::Array2D( std::vector< std::vector< T > > const & array )
  : Array2D {}
{
    if ( array.empty() )
    {
        return;
    }

    if ( ! vector::is_rectangle( array ) )
    {
        // TODO handle non-rectangle array by settings default values to the
        // missing elements
        Trace::Warning( "Each columns must have the same size" );
    }

    m_size = math::Vector2U { array[0].size(), array.size() };
    for ( std::vector< T > const & subvector : array )
    {
        vector::append( m_array2D, subvector );
    }
}

template< typename T >
math::Vector2U Array2D< T >::size() const
{
    return m_size;
}

template< typename T >
void Array2D< T >::resize( math::Vector2U size, T defaultValue )
{
    std::vector< T > newArray {};
    newArray.assign( size.x * size.y, defaultValue );

    unsigned int minRows = std::min( m_size.y, size.y );
    unsigned int minCols = std::min( m_size.x, size.x );
    for ( unsigned int i = 0u; i < minCols; ++i )
    {
        for ( unsigned int j = 0u; j < minRows; ++j )
        {
            newArray[j * size.x + i] = m_array2D[j * m_size.x + i];
        }
    }

    m_size    = size;
    m_array2D = std::move( newArray );
}

template< typename T >
std::vector< T > const & Array2D< T >::raw_array() const
{
    return m_array2D;
}

template< typename T >
T const & Array2D< T >::operator() ( unsigned int col, unsigned int row ) const
{
    return const_cast< Array2D< T > * >( this )->operator() ( col, row );
}

template< typename T >
T & Array2D< T >::operator() ( unsigned int col, unsigned int row )
{
    if ( col >= m_size.x || row >= m_size.y )
    {
        Trace::Error( "Out of range" );
    }

    return m_array2D[this->get_index( col, row )];
}

// template< typename T >
// std::vector< T > const Array2D< T >::operator[] ( unsigned int line ) const
// {
//     if ( line >= m_size.y )
//     {
//         Trace::Error( "Line  out of range for 2D array of size " );
//     }

// return vector::extract( m_array2D, line * m_size.x,
//                         line * ( m_size.x + 1 ) );
// }

template< typename T >
math::Vector2U Array2D< T >::get_position( unsigned int index ) const
{
    return math::Vector2U {
        static_cast< unsigned int >( index % m_size.x,
                                     std::floor( index / m_size.x ) ) };
}

template< typename T >
unsigned int Array2D< T >::get_index( unsigned int col, unsigned int row ) const
{
    return col + row * m_size.x;
}

template< typename T >
unsigned int Array2D< T >::get_last_index() const
{
    return m_size.x * m_size.y - 1;
}

template< typename T >
std::ostream & operator<< ( std::ostream & stream, Array2D< T > const & array )
{
    return stream << "{ " << array.m_size << " | " << array.m_array2D << " }";
}

template< typename T >
std::istream & operator>> ( std::istream & stream, Array2D< T > & array )
{
    stream::ignore_next( stream, '{' );
    stream >> array.m_size;
    stream::ignore_next( stream, '|' );
    stream >> array.m_array2D;
    stream::ignore_next( stream, '}' );

    ASSERTION( array.m_size.x * array.m_size.y == array.m_array2D.size(), "" );

    return stream;
}
