#pragma once

#include <array>
#include <cmath>

#include "graphics2D/sfml.hpp"
#include "tools/assertion.hpp"
#include "tools/imgui.hpp"

namespace math
{
    struct Rectangle;

    float cosinus_radian( float const & value );
    float cosinus_degree( float const & value );
    float sinus_radian( float const & value );
    float sinus_degree( float const & value );

    float degree_to_radian( float const & degree );
    float radian_to_degree( float const & radian );

    struct Vector2D
    {
        float x { 0.f };
        float y { 0.f };

        constexpr Vector2D() = default;
        constexpr Vector2D( float const & xAxisValue, float const & yAxisValue )
          : x( xAxisValue ), y( yAxisValue )
        {}
        constexpr Vector2D( int const & xAxisValue, int const & yAxisValue )
          : Vector2D( static_cast< float >( xAxisValue ),
                      static_cast< float >( yAxisValue ) )
        {}
        constexpr Vector2D( unsigned int const & xAxisValue,
                            unsigned int const & yAxisValue )
          : Vector2D( static_cast< float >( xAxisValue ),
                      static_cast< float >( yAxisValue ) )
        {}
        // constexpr Vector2D( uns const & xAxisValue,
        //                     size_t const & yAxisValue )
        //   : Vector2D( static_cast< float >( xAxisValue ),
        //               static_cast< float >( yAxisValue ) )
        // {}
        Vector2D( sf::Vector2f const & sfmlVector );
        Vector2D( sf::Vector2u const & sfmlVector );
        Vector2D( ImVec2 const & imGuiVector );
        constexpr virtual ~Vector2D() = default;

        virtual float operator[]( std::size_t index ) const;

        operator sf::Vector2f() const;
        operator ImVec2() const;

        bool is_inside( Rectangle const & rectangle ) const;
        bool is_inside( Vector2D const & position,
                        Vector2D const & size ) const;
    };

    std::ostream & operator<<( std::ostream & stream,
                               Vector2D const & vector2D );

    Vector2D operator*( Vector2D const & vector2DLeft,
                        Vector2D const & vector2DRight );
    Vector2D operator/( Vector2D const & vector2DLeft,
                        Vector2D const & vector2DRight );
    Vector2D operator+( Vector2D const & vector2DLeft,
                        Vector2D const & vector2DRight );
    Vector2D operator-( Vector2D const & vector2DLeft,
                        Vector2D const & vector2DRight );

    Vector2D operator*( Vector2D const & vector2D, float const & factor );
    Vector2D operator/( Vector2D const & vector2D, float const & factor );

    Vector2D operator-( Vector2D const & vector2D );

    Vector2D operator-=( Vector2D & vector2DLeft,
                         Vector2D const & vector2DRight );

    /**
     * @brief Check if a vector is between the position and position + size
     */
    bool is_inside( Vector2D const & value, Vector2D const & position,
                    Vector2D const & size );

    struct Vector3D : public Vector2D
    {
        float z { 0.f };

        Vector3D() = default;
        Vector3D( float const & xAxisValue, float const & yAxisValue,
                  float const & zAxisValue );
        virtual ~Vector3D() = default;

        float get_length() const;
        Vector3D get_direction() const;
        void set_direction( Vector3D const & direction );

        virtual float operator[]( std::size_t index ) const override;

        void normalize();
        void rotate( float const & angle, Vector3D const & directionAxis );
    };

    Vector3D operator*( Vector3D const & vectorLeft,
                        Vector3D const & vectorRight );

    Vector3D operator*( float const & factor, Vector3D const & vector );
    Vector3D operator/( float const & factor, Vector3D const & vector );
    Vector3D operator+( float const & summand, Vector3D const & vector );
    Vector3D power( Vector3D const & vector, int const & powerFactor );

    struct Rectangle
    {
        Vector2D position { 0.f, 0.f };
        Vector2D size { 0.f, 0.f };

        Rectangle() = default;
        Rectangle( Vector2D const & aPosition, Vector2D const & aSize );
        Rectangle( float const & x, float const & y, float const & width,
                   float const & height );
        virtual ~Rectangle() = default;
    };

    std::ostream & operator<<( std::ostream & stream, Vector3D const & vector );

    class Matrix2
    {
      public:
        Matrix2( Vector2D const & vectorLine1, Vector2D const & vectorLine2 );
        virtual ~Matrix2() = default;

        Vector2D operator[]( std::size_t index ) const;
        float get_determining_factor() const;

      private:
        std::array< Vector2D, 2 > m_vectorArray;
    };
} // namespace math