#pragma once
#include "polygon.hpp"

#include <sstream>

namespace math
{
    template < C_Primitive Type, unsigned int NbOfPoints >
    Polygon< Type, NbOfPoints >::Polygon() : m_points()
    {
        m_points.fill( Point< Type > { 0, 0 } );
    }

    // template < C_Primitive Type, unsigned int NbOfPoints >
    // Polygon< Type, NbOfPoints >::Polygon( Point< Type > a, Point< Type > b,
    //                                        Point< Type > c, Point< Type > d )
    //   : topLeftPosition( a ),
    //     topRightPosition( b ),
    //     bottomRightPosition( c ),
    //     bottomLeftPosition( d )
    // {}

    template < C_Primitive Type, unsigned int NbOfPoints >
    Polygon< Type, NbOfPoints >::Polygon(
        Rectangle< Type > rectangle ) requires( C_Quadrangle< NbOfPoints > )
      : Polygon {}
    {
        this->set_rectangle( rectangle );
    }

    template < C_Primitive Type, unsigned int NbOfPoints >
    Point< Type > Polygon< Type, NbOfPoints >::operator[](
        unsigned int index ) const
    {
        if ( index >= NbOfPoints )
        {
            throw std::runtime_error { "Polygon index out of range" };
        }

        return m_points[index];
    }

    template < C_Primitive Type, unsigned int NbOfPoints >
    Point< Type > & Polygon< Type, NbOfPoints >::operator[](
        unsigned int index )
    {
        if ( index >= NbOfPoints )
        {
            throw std::runtime_error { "Polygon index out of range" };
        }

        return m_points[index];
    }

    template < C_Primitive Type, unsigned int NbOfPoints >
    unsigned int Polygon< Type, NbOfPoints >::get_number_of_point() const
    {
        ASSERTION( NbOfPoints == m_points.size() );

        return NbOfPoints;
    }

    template < C_Primitive Type, unsigned int NbOfPoints >
    void Polygon< Type, NbOfPoints >::set_rectangle(
        Rectangle< Type > rectangle ) requires( C_Quadrangle< NbOfPoints > )
    {
        m_points[0] = rectangle.position;
        m_points[1] = rectangle.position + rectangle.size.get_x_axis();
        m_points[2] = rectangle.position + rectangle.size;
        m_points[3] = rectangle.position + rectangle.size.get_y_axis();
    }

    template < C_Primitive Type, unsigned int NbOfPoints >
    std::string Polygon< Type, NbOfPoints >::print() const
    {
        std::stringstream stream {};

        stream << this->get_number_of_point() << " points ("
               << "\n";
        for ( auto point : m_points )
        {
            stream << "\t" << point << "\n";
        }
        stream << ")";

        return stream.str();
    }

    template < C_Primitive Type, unsigned int NbOfPoints >
    std::array< Segment< Type >, NbOfPoints >
        Polygon< Type, NbOfPoints >::get_segments() const
    {
        std::array< Segment< Type >, NbOfPoints > segments {};

        for ( unsigned int i_point { 0 }; i_point < this->get_number_of_point();
              ++i_point )
        {
            unsigned int i_pointNext { ( i_point + 1 )
                                       % this->get_number_of_point() };
            segments[i_point] = { ( *this )[i_point], ( *this )[i_pointNext] };
        }

        return segments;
    }
    template < C_Primitive Type, unsigned int NbOfPoints >
    std::array< Point< Type >, NbOfPoints >
        Polygon< Type, NbOfPoints >::get_points() const
    {
        return m_points;
    }

    template < C_Primitive Type, unsigned int NbOfPoints >
    bool is_intersection( Polygon< Type, NbOfPoints > polygonLeft,
                          Polygon< Type, NbOfPoints > polygonRight )
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