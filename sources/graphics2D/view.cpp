#include "view.hpp"

#include <stdexcept>  // for logic_error
#include <string>     // for string_literals

// using namespace std::string_literals;
#include "interface/window.hpp"  // for Window
#include "libraries/imgui.hpp"   // for ImGui::Begin, ImGui::End, ImGui::Text
#include "maths/vector2.tpp"     // for operator/, Vector2::Vector2<Type>
#include "tools/singleton.tpp"   // for Singleton::get_instance

View::View() : View { sf::View {} } {}

// ? Why do we put noexcept ? Comment the answer
View::View( sf::View const & sfView ) noexcept
  : DebugWindow { "view" },
    m_sfView { sfView },
    m_zoomMax { 15.f },
    m_zoomMin { 0.7f }
{}

View::operator sf::View & ()
{
    return m_sfView;
}

View::operator sf::View const & () const
{
    return m_sfView;
}

math::Vector2F View::get_zoom( math::Vector2F windowSize ) const
{
    return windowSize / math::Vector2F { m_sfView.getSize() };
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

    m_sfView.setSize( windowSize / newZoom );
    this->add_debug_text( "Zoom : {}", newZoom );
}

void View::set_zoom( float newZoom, Window const & window )
{
    this->set_zoom( newZoom, window.get_size().to_float() );
}

void View::zoom( float factor, math::Vector2F windowSize )
{
    m_sfView.zoom( 1.f - factor );
    this->add_debug_text( "Zoom : ", 1.f - factor );

    if ( this->get_zoom( windowSize ).get_max() > m_zoomMax )
    {
        this->set_zoom( m_zoomMax, windowSize );
    }
    if ( this->get_zoom( windowSize ).get_min() < m_zoomMin )
    {
        this->set_zoom( m_zoomMin, windowSize );
    }
}

void View::zoom( float factor, Window const & window )
{
    this->zoom( factor, window.get_size().to_float() );
}

math::Vector2F View::get_size() const
{
    return m_sfView.getSize();
}

math::Vector2F View::get_center() const
{
    return m_sfView.getCenter();
}

math::PointF View::get_position() const
{
    return this->get_center() - ( this->get_size() / 2.f );
}

math::RectangleF View::get_rectangle() const
{
    return math::RectangleF { this->get_position(), this->get_size() };
}

float View::get_rotation() const
{
    return m_sfView.getRotation();
}

math::RectangleF View::get_viewport() const
{
    return m_sfView.getViewport();
}

bool View::contain( math::PointF point ) const
{
    return point.is_inside( this->get_rectangle() );
}

void View::debug_window_content()
{
    ImGui::TextFmt( "View Position : {}", this->get_position().to_vector() );
    ImGui::TextFmt( "View Size : {}", this->get_size() );
    // ImGui::TextFmt( "View Rectangle : {}", this->get_rectangle() );
    // ImGui::Text( "View Zoom : {}", this->get_zoom( Window ) );
    ImGui::TextFmt( "View Center : {}", this->get_center() );
    ImGui::TextFmt( "View Rotation : {}", this->get_rotation() );
    // ImGui::TextFmt( "View Viewport : {}", this->get_viewport() );
    ImGui::TextFmt( "View Zoom Min : {}", m_zoomMin );
    ImGui::TextFmt( "View Zoom Max : {}", m_zoomMax );
}
