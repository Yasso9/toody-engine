#include "view.hpp"

#include <stdexcept>  // for logic_error
#include <string>     // for string_literals

using namespace std::string_literals;
#include "interface/window.hpp"  // for Window
#include "maths/vector2.tpp"     // for operator/, Vector2::Vector2<Type>
#include "tools/singleton.tpp"   // for Singleton::get_instance

math::Vector2F View::get_zoom( math::Vector2F windowSize ) const
{
    return windowSize / math::Vector2F { this->getSize() };
}

math::Vector2F View::get_zoom( Window const & window ) const
{
    return this->get_zoom( window.get_size().to_float() );
}

void View::set_zoom( float newZoom, math::Vector2F windowSize )
{
    if ( newZoom == 0 )
    {
        throw std::logic_error { "Division by Zero" };
    }

    this->setSize( windowSize / newZoom );
}

void View::set_zoom( float newZoom, Window const & window )
{
    this->set_zoom( newZoom, window.get_size().to_float() );
}

void View::zoom( float factor, math::Vector2F windowSize )
{
    sf::View::zoom( 1.f - factor );

    constexpr float MAXIMUM_ZOOM { 15.f };
    constexpr float MINIMUM_ZOOM { 0.7f };
    if ( this->get_zoom( windowSize ).get_max() > MAXIMUM_ZOOM )
    {
        this->set_zoom( MAXIMUM_ZOOM, windowSize );
    }
    if ( this->get_zoom( windowSize ).get_min() < MINIMUM_ZOOM )
    {
        this->set_zoom( MINIMUM_ZOOM, windowSize );
    }
}

void View::zoom( float factor, Window const & window )
{
    this->zoom( factor, window.get_size().to_float() );
}

math::Vector2F View::get_size() const
{
    return this->getSize();
}

math::Vector2F View::get_center() const
{
    return this->getCenter();
}

math::PointF View::get_position() const
{
    return this->get_center() - ( this->get_size() / 2.f );
}

math::RectangleF View::get_rectangle() const
{
    return math::RectangleF { this->get_position(), this->get_size() };
}

bool View::contain( math::PointF point ) const
{
    return point.is_inside( this->get_rectangle() );
}
