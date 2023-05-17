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
        std::cerr << "Size and Array not comptatible" << std::endl;
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
void Array2D< T >::set_size( math::Vector2U size, T defaultValue )
{
    // Changment in lines, we add element in columns
    if ( m_size.y < size.y )
    {
        // Differences between the 2 sizes
        unsigned int diff = size.y - m_size.y;

        // We add lines at the end of the array so we can just append a vector
        // of the desized size
        std::vector< T > newVector {};
        newVector.assign( diff * m_size.x, defaultValue );

        vector::append( m_array2D, newVector );

        // Update the size
        m_size.y = size.y;
    }
    else if ( m_size.y > size.y )
    {
        m_size.y = size.y;

        // We remove lines at the end of the array so we erase element to the
        // specified index
        m_array2D.erase( m_array2D.begin() + this->get_last_index() + 1,
                         m_array2D.end() );
    }

    // Changment in columns, we add element in lines
    if ( m_size.x < size.x )
    {
        // Number of element to add in each lines
        unsigned int diff = size.x - m_size.x;

        for ( unsigned int index = m_size.x; index <= size.x * m_size.y;
              index += size.x )
        {
            m_array2D.insert( m_array2D.begin() + index, diff, defaultValue );
        }
    }
    else if ( m_size.x > size.x )
    {
        int diff = static_cast< int >( m_size.x - size.x );

        for ( int index = static_cast< int >( this->get_last_index() ) + 1;
              index >= diff; index -= static_cast< int >( m_size.x ) )
        {
            m_array2D.erase( m_array2D.begin() + index - diff,
                             m_array2D.begin() + index );
        }
    }

    m_size.x = size.x;

    if ( m_size.x * m_size.y != m_array2D.size() )
    {
        std::cerr << "Size " << m_size
                  << " isn't comptatible with an vector of size "
                  << m_array2D.size() << std::endl;
    }
}

template< typename T >
std::vector< T > const & Array2D< T >::get_raw_array() const
{
    return m_array2D;
}

template< typename T >
std::vector< T > const Array2D< T >::operator[] ( unsigned int line ) const
{
    if ( line >= m_size.y )
    {
        std::cerr << "Line " << line << " out of range for 2D array of size "
                  << m_size << std::endl;
        return {};
    }

    return vector::extract( m_array2D, line * m_size.x,
                            line * ( m_size.x + 1 ) );
}

template< typename T >
T const & Array2D< T >::get_element( unsigned int column,
                                     unsigned int line ) const
{
    return const_cast< Array2D< T > * >( this )->get_element( column, line );
}

template< typename T >
T & Array2D< T >::get_element( unsigned int column, unsigned int line )
{
    if ( column >= m_size.x || line >= m_size.y )
    {
        std::cerr << "Element " << math::Vector2 { column, line }
                  << " too big for array of size " << m_size << std::endl;
    }

    return m_array2D[this->get_index( column, line )];
}

template< typename T >
math::Vector2U Array2D< T >::get_position( unsigned int index ) const
{
    return math::Vector2U { static_cast< unsigned int >(
        index % m_size.x, std::floor( index / m_size.x ) ) };
}

template< typename T >
unsigned int Array2D< T >::get_index( unsigned int column,
                                      unsigned int line ) const
{
    return column + line * m_size.x;
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
