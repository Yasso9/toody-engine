#pragma once

#include <vector>

#include "maths/vector2.hpp"

template< typename Type >
class Array2D
{
  protected:
    std::vector< Type > m_array2D;
    /// @brief x : index of columns
    ///        y : index of lines
    math::Vector2U      m_size;

  public:
    Array2D();
    Array2D( std::vector< Type > array, math::Vector2U size );
    Array2D( std::vector< std::vector< Type > > array );
    virtual ~Array2D() = default;

    /// @brief Get the number of column (x) and lines (y) of the array
    math::Vector2U              get_size () const;
    std::vector< Type > const & get_raw_array () const;
    Type const & get_element ( unsigned int column, unsigned int line ) const;
    Type &       get_element ( unsigned int column, unsigned int line );

    math::Vector2U get_position ( unsigned int index ) const;
    unsigned int   get_index ( unsigned int column, unsigned int line ) const;
    unsigned int   get_last_index () const;

    std::vector< Type > const operator[] ( unsigned int line ) const;

    void set_size ( math::Vector2U size, Type defaultValue = Type {} );

    template< typename TypeBis >
    friend std::ostream & operator<< ( std::ostream &             stream,
                                       Array2D< TypeBis > const & array );
    template< typename TypeBis >
    friend std::istream & operator>> ( std::istream &       stream,
                                       Array2D< TypeBis > & array );
};

#include "array2D.tpp"