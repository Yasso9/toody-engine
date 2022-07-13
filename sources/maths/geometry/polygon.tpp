#pragma once
#include "polygon.hpp"

#include <sstream>

namespace math
{
    template < C_Primitive Type >
    Polygon< Type >::Polygon() : m_points()
    {}

    template < C_Primitive Type >
    Polygon< Type >::Polygon( Rectangle< Type > rectangle ) : Polygon {}
    {
        this->set_rectangle( rectangle );
    }

    template < C_Primitive Type >
    Point< Type > Polygon< Type >::operator[]( unsigned int index ) const
    {
        if ( index >= this->get_number_of_points() )
        {
            throw std::overflow_error { "Polygon index out of range" };
        }

        return m_points[index];
    }

    template < C_Primitive Type >
    Point< Type > & Polygon< Type >::operator[]( unsigned int index )
    {
        if ( index >= this->get_number_of_points() )
        {
            throw std::overflow_error { "Polygon index out of range" };
        }

        return m_points[index];
    }

    template < C_Primitive Type >
    unsigned int Polygon< Type >::get_number_of_points() const
    {
        return static_cast< unsigned int >( m_points.size() );
    }

    template < C_Primitive Type >
    void Polygon< Type >::set_rectangle( Rectangle< Type > rectangle )
    {
        m_points.resize( 4 );

        m_points[0] = rectangle.position;
        m_points[1] = rectangle.position + rectangle.size.get_x_axis();
        m_points[2] = rectangle.position + rectangle.size;
        m_points[3] = rectangle.position + rectangle.size.get_y_axis();
    }

    template < C_Primitive Type >
    std::string Polygon< Type >::print() const
    {
        std::stringstream stream {};

        stream << this->get_number_of_points() << " points ["
               << "\n";
        for ( auto point : m_points )
        {
            stream << "\t" << point << ",\n";
        }
        stream << "]";

        return stream.str();
    }

    template < C_Primitive Type >
    std::vector< Segment< Type > > Polygon< Type >::get_segments() const
    {
        std::vector< Segment< Type > > segments {};

        for ( unsigned int i_point = 0u; i_point < this->get_number_of_points();
              ++i_point )
        {
            unsigned int i_pointNext { ( i_point + 1 )
                                       % this->get_number_of_points() };
            segments[i_point] = { ( *this )[i_point], ( *this )[i_pointNext] };
        }

        return segments;
    }
    template < C_Primitive Type >
    std::vector< Point< Type > > Polygon< Type >::get_points() const
    {
        return m_points;
    }

    template < C_Primitive Type >
    bool is_intersection( Polygon< Type > polygonLeft,
                          Polygon< Type > polygonRight )
    {
        for ( Point< Type > pointPolygonLeft : polygonLeft.get_points() )
        {
            if ( pointPolygonLeft.is_inside( polygonRight ) )
            {
                return true;
            }
        }
        for ( Point< Type > pointPolygonRight : polygonRight.get_points() )
        {
            if ( pointPolygonRight.is_inside( polygonLeft ) )
            {
                return true;
            }
        }

        return false;
    }
} // namespace math