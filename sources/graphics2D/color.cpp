#include "color.hpp"

#include "graphics2D/constants.hpp"  // for COLOR_RANGE
#include "tools/assertion.hpp"       // for ASSERTION

Color::RGBA::RGBA() : RGBA { 0u, 0u, 0u, 0u } {}

Color::RGBA::RGBA( unsigned int red, unsigned int green, unsigned int blue,
                   unsigned int opacity )
  : r { red }, g { green }, b { blue }, a { opacity }
{}

Color::Color() : Color { RGBA {} } {}

Color::Color( RGBA rgbaColor ) : m_table { 0.f, 0.f, 0.f, 0.f }
{
    this->set( rgbaColor );
}

uint32_t Color::to_integer() const
{
    Color::RGBA rgbaColor { this->to_rgba() };

    return ( static_cast< ImU32 >( rgbaColor.a ) << 24 )
           | ( static_cast< ImU32 >( rgbaColor.b ) << 16 )
           | ( static_cast< ImU32 >( rgbaColor.g ) << 8 )
           | ( static_cast< ImU32 >( rgbaColor.r ) << 0 );
}

float * Color::to_table()
{
    return &( m_table[0] );
}

sf::Color Color::to_sfml() const
{
    Color::RGBA rgbaColor { this->to_rgba() };

    return { static_cast< sf::Uint8 >( rgbaColor.r ),
             static_cast< sf::Uint8 >( rgbaColor.g ),
             static_cast< sf::Uint8 >( rgbaColor.b ),
             static_cast< sf::Uint8 >( rgbaColor.a ) };
}

Color::RGBA Color::to_rgba() const
{
    RGBA rgbaColor {
        /// @todo maybe use round or floor
        static_cast< unsigned int >( m_table[0] * COLOR_RANGE_F ),
        static_cast< unsigned int >( m_table[1] * COLOR_RANGE_F ),
        static_cast< unsigned int >( m_table[2] * COLOR_RANGE_F ),
        static_cast< unsigned int >( m_table[3] * COLOR_RANGE_F ) };

    return rgbaColor;
}

void Color::set( Color::RGBA rgbaColor )
{
    m_table[0] = static_cast< float >( rgbaColor.r ) / COLOR_RANGE_F;
    m_table[1] = static_cast< float >( rgbaColor.g ) / COLOR_RANGE_F;
    m_table[2] = static_cast< float >( rgbaColor.b ) / COLOR_RANGE_F;
    m_table[3] = static_cast< float >( rgbaColor.a ) / COLOR_RANGE_F;
};
