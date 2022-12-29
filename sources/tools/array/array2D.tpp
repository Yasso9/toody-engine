#pragma once

#include "array2D.hpp"

#include "tools/array/vector.hpp"

template< typename Type >
Array2D< Type >::Array2D() : m_array2D {}, m_size { 0u, 0u }
{}

template< typename Type >
Array2D< Type >::Array2D( std::vector< Type > array, math::Vector2U size )
  : m_array2D { array }, m_size { size }
{
    if ( array.size() != size.x * size.y )
    {
        std::cerr << "Size and Array not comptatible" << std::endl;
    }
}

template< typename Type >
Array2D< Type >::Array2D( std::vector< std::vector< Type > > array )
  : Array2D {}
{
    if ( array.empty() )
    {
        return;
    }

    /// @todo don't check that and add default value of all missing value in
    /// rectangle if the default contructor exist
    if ( ! vector::is_rectangle( array ) )
    {
        std::cerr << "Each columns must have the same size" << std::endl;
    }

    m_size = math::Vector2U { array[0].size(), array.size() };

    for ( std::vector< Type > const & subvector : array )
    {
        vector::append( m_array2D, subvector );
    }
}

template< typename Type >
math::Vector2U Array2D< Type >::get_size() const
{
    return m_size;
}

template< typename Type >
std::vector< Type > const & Array2D< Type >::get_raw_array() const
{
    return m_array2D;
}

template< typename Type >
std::vector< Type > const Array2D< Type >::operator[] (
    unsigned int line ) const
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

template< typename Type >
Type const & Array2D< Type >::get_element( unsigned int line,
                                           unsigned int column ) const
{
    return const_cast< Array2D< Type > * >( this )->get_element( line, column );
}

template< typename Type >
Type & Array2D< Type >::get_element( unsigned int line, unsigned int column )
{
    return m_array2D[this->get_index( line, column )];
}

template< typename Type >
unsigned int Array2D< Type >::get_index( unsigned int column,
                                         unsigned int line )
{
    return column + line * m_size.x;
}

template< typename Type >
void Array2D< Type >::set_size( math::Vector2U size, Type defaultValue )
{
    std::cout << "Array base " << m_array2D << std::endl;

    // Changment in columns
    if ( this->get_size().x < size.x )
    {
        unsigned int diff = size.x - this->get_size().x;

        for ( unsigned int index = size.x; index < size.x * this->get_size().y;
              index += size.x )
        {
            m_array2D.insert( m_array2D.begin() + index, diff, defaultValue );
        }
    }
    else if ( this->get_size().x > size.x )
    {
        unsigned int diff = this->get_size().x - size.x;

        for ( unsigned int index = size.x; index < size.x * this->get_size().y;
              index += this->get_size().x )
        {
            m_array2D.erase( m_array2D.begin() + index,
                             m_array2D.begin() + index + diff );
        }
    }

    std::cout << "Array X " << m_array2D << std::endl;

    m_size.x = size.x;

    // Changment in lines
    if ( this->get_size().y < size.y )
    {
        unsigned int diff = size.y - this->get_size().y;

        std::vector< Type > newVector {};
        newVector.assign( diff * this->get_size().x, defaultValue );

        std::cout << "New Vector " << newVector << std::endl;

        vector::append( m_array2D, newVector );
    }
    else if ( this->get_size().y > size.y )
    {
        // Index
        unsigned int lastElement = this->get_size().x * size.y;

        m_array2D.erase( m_array2D.begin() + lastElement, m_array2D.end() );
    }

    std::cout << "Array Y " << m_array2D << std::endl;

    m_size.y = size.y;
}

template< typename Type >
std::ostream & operator<< ( std::ostream &          stream,
                            Array2D< Type > const & array )
{
    return stream << "{ " << array.m_size << " | " << array.m_array2D << " }";
}

template< typename Type >
std::istream & operator>> ( std::istream & stream, Array2D< Type > & array )
{
    stream::ignore_next( stream, '{' );
    stream >> array.m_size;
    stream::ignore_next( stream, '|' );
    stream >> array.m_array2D;
    stream::ignore_next( stream, '}' );
    return stream;
}