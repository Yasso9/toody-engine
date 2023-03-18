#pragma once

#include <array>
#include <cstdint>

namespace sf
{
    class Color;
}  // namespace sf

class Color
{
    std::array< float, 4 > m_table;

  public:
    class RGBA
    {
      public:
        unsigned int r;
        unsigned int g;
        unsigned int b;
        unsigned int a;

        RGBA();
        RGBA( unsigned int r, unsigned int g, unsigned int b, unsigned int a );
        virtual ~RGBA() = default;
    };

    Color();
    Color( RGBA rgbaColor );
    virtual ~Color() = default;

    uint32_t  to_integer () const;
    float *   to_table ();
    sf::Color to_sfml () const;
    RGBA      to_rgba () const;

    void set ( RGBA rgbaColor );
};