#pragma once

#include <vector>

#include "maths/vector2.hpp"

template< typename T >
class Array2D
{
  protected:
    std::vector< T > m_array2D;
    // x : index of columns
    // y : index of lines
    math::Vector2U   m_size;

  public:
    Array2D();
    Array2D( std::vector< T > const & array, math::Vector2U size );
    Array2D( std::vector< std::vector< T > > const & array );
    virtual ~Array2D() = default;

    // Number of columns and lines
    math::Vector2U size () const;
    void           resize ( math::Vector2U size, T defaultValue = T {} );

    std::vector< T > const & get_raw_array () const;
    T const & operator() ( unsigned int col, unsigned int row ) const;
    T &       operator() ( unsigned int col, unsigned int row );

    std::vector< T > const operator[] ( unsigned int row ) const;
    std::vector< T > const get_row ( unsigned int row ) const;
    std::vector< T > const get_col ( unsigned int col ) const;

    math::Vector2U get_position ( unsigned int index ) const;
    unsigned int   get_index ( unsigned int column, unsigned int line ) const;
    unsigned int   get_last_index () const;

    template< typename T2 >
    friend std::ostream & operator<< ( std::ostream &        stream,
                                       Array2D< T2 > const & array );
    template< typename T2 >
    friend std::istream & operator>> ( std::istream &  stream,
                                       Array2D< T2 > & array );
};

#include "array2D.tpp"
