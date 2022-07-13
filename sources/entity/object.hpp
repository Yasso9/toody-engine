#pragma once

#include "input/movement_key.hpp"

class StaticEntity2D : public sf::Shape
{
  protected:
    math::PolygonF m_polygon;

  public:
    explicit StaticEntity2D( math::PolygonF polygon );
    ~StaticEntity2D() = default;

    unsigned int get_point_count() const
    {
        return m_polygon.get_number_of_points();
    }
    math::Vector2F get_point( unsigned int index ) const
    {
        return m_polygon[index];
    }

    math::PointF get_position() const;
    math::PolygonF get_polygon( bool getSize = true ) const;

    void set_polygon( math::PolygonF polygon );

    bool is_intersected_by( StaticEntity2D const & otherEntity ) const;

  private:
    std::size_t getPointCount() const override
    {
        return static_cast< std::size_t >( this->get_point_count() );
    }
    sf::Vector2f getPoint( std::size_t index ) const override
    {
        return this->get_point( static_cast< unsigned int >( index ) );
    }
};

using T_CollisionMap = std::vector< StaticEntity2D >;

/// @brief Moveable Entity
class Entity2D : public StaticEntity2D
{
    T_CollisionMap const & m_collisionMap;
    keyboard_move::S_Key m_movementKey;
    math::Vector2F m_speed;

  public:
    Entity2D( math::PolygonF quadrangle, T_CollisionMap const & collisionMap,
              keyboard_move::S_Key movementKey );

    math::Vector2F get_speed() const;

    void set_speed( float speed );
    void set_speed( math::Vector2F speed );

    bool is_collision_detected() const;

    void update( float deltaTime,
                 math::Vector2F viewZoom = math::Vector2F( 1.f, 1.f ) );
};

namespace customize
{
    static void circle_shape( sf::CircleShape & circleShape )
    {
        /// @todo check if a window has begin

        sf::Color background { circleShape.getFillColor() };
        sf::Color outline { circleShape.getOutlineColor() };
        float outlineThickness { circleShape.getOutlineThickness() };
        float radius { circleShape.getRadius() };
        std::size_t numberOfPoint { circleShape.getPointCount() };

        ImGui::P_ColorEditor( "Background Color", background );
        ImGui::P_ColorEditor( "Outline Color", outline );
        ImGui::InputFloat( "Outline Thickness", &outlineThickness );
        ImGui::InputFloat( "Radius", &radius );
        /// @todo if doesn't work, cast it to unsigned int
        ImGui::P_InputNumber( "Number of Point", numberOfPoint );

        circleShape.setFillColor( background );
        circleShape.setOutlineColor( outline );
        circleShape.setOutlineThickness( outlineThickness );
        circleShape.setRadius( radius );
        circleShape.setPointCount( numberOfPoint );
    }
} // namespace customize

/// @brief Entity that can be customised
class CustomEntity2D : public StaticEntity2D
{
    bool m_showCustomisation;
    bool m_editPoint;
    sf::CircleShape m_pointShape;

  public:
    CustomEntity2D( math::PolygonF quadrangle )
      : StaticEntity2D( quadrangle ),
        m_showCustomisation( true ),
        m_editPoint( false ),
        m_pointShape { 5.f, 40 }
    {}

    void update_customisation()
    {
        if ( ! m_showCustomisation )
        {
            return;
        }

        if ( ImGui::P_Begin( "Customise Entity", &m_showCustomisation ) )
        {
            customize::circle_shape( m_pointShape );

            ImGui::Checkbox( "Edit Points ?", &m_editPoint );
        }
        ImGui::End();
    }

  private:
    void draw( sf::RenderTarget & target,
               sf::RenderStates states ) const override
    {
        target.draw( *this, states );

        sf::CircleShape pointShape { m_pointShape };
        for ( math::PointF const & point : m_polygon.get_points() )
        {
            pointShape.setPosition( point.x, point.y );
            target.draw( pointShape, states );
        }
    }
};