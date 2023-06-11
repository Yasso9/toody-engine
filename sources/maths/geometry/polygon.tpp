#pragma once

#include "polygon.hpp"

#include <sstream>

#include "tools/stream/stream.hpp"

namespace math
{
    template< cPrimitive Type >
    Polygon< Type >::Polygon() : m_points()
    {}

    template< cPrimitive Type >
    Polygon< Type >::Polygon( Rectangle< Type > rectangle ) : Polygon {}
    {
        this->set_rectangle( rectangle );
    }

    template< cPrimitive Type >
    Point< Type > const & Polygon< Type >::operator[] (
        unsigned int index ) const
    {
        if ( index >= this->get_number_of_points() )
        {
            throw std::overflow_error { "Polygon index out of range" };
        }

        return m_points[index];
    }

    template< cPrimitive Type >
    Point< Type > & Polygon< Type >::operator[] ( unsigned int index )
    {
        return const_cast< Point< Type > & >(
            const_cast< Polygon< Type > const * >( *this )->operator[] (
                index ) );
    }

    template< cPrimitive Type >
    unsigned int Polygon< Type >::get_number_of_points() const
    {
        return static_cast< unsigned int >( m_points.size() );
    }

    template< cPrimitive Type >
    void Polygon< Type >::set_rectangle( Rectangle< Type > rectangle )
    {
        m_points.resize( 4 );

        m_points[0] = rectangle.pos;
        m_points[1] = rectangle.pos + rectangle.size.get_x_axis();
        m_points[2] = rectangle.pos + rectangle.size;
        m_points[3] = rectangle.pos + rectangle.size.get_y_axis();
    }

    template< cPrimitive Type >
    std::string Polygon< Type >::print() const
    {
        std::ostringstream stream {};

        stream << this->get_number_of_points() << " points ["
               << "\n";
        for ( math::Point< Type > point : m_points )
        {
            stream << "\t" << point << ",\n";
        }
        stream << "]";

        return stream.str();
    }

    template< cPrimitive Type >
    std::vector< Line< Type > > Polygon< Type >::get_segments() const
    {
        std::vector< Line< Type > > segments {};
        // Set the number of elements that the vector should contain
        segments.resize( this->get_number_of_points() );

        for ( unsigned int i_point = 0u; i_point < this->get_number_of_points();
              ++i_point )
        {
            unsigned int i_pointNext { ( i_point + 1 )
                                       % this->get_number_of_points() };
            segments[i_point] = math::Line< Type > { ( *this )[i_point],
                                                     ( *this )[i_pointNext] };
        }

        return segments;
    }

    template< cPrimitive Type >
    std::vector< Point< Type > > Polygon< Type >::get_points() const
    {
        return const_cast< Polygon * >( this )->get_points();
    }

    template< cPrimitive Type >
    std::vector< Point< Type > > & Polygon< Type >::get_points()
    {
        return m_points;
    }

    template< cPrimitive Type >
    Polygon< Type > Polygon< Type >::operator+ ( math::Vector2< Type > vector )
    {
        Polygon< Type > newPolygon { *this };

        for ( math::Point< Type > & point : newPolygon.get_points() )
        {
            point += vector;
        }

        return newPolygon;
    }

    template< cPrimitive Type >
    std::ostream & operator<< ( std::ostream &          stream,
                                Polygon< Type > const & polygon )
    {
        stream << "{ ";
        for ( Point< Type > point : polygon.get_points() )
        {
            stream << point << "; ";
        }
        stream << "}";

        return stream;
    }

    template< cPrimitive Type >
    bool is_intersection ( Polygon< Type > polygonLeft,
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

}  // namespace math
