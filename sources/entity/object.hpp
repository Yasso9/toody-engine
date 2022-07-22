#pragma once

#include "graphics2D/component.hpp"
#include "graphics2D/view.hpp"
#include "input/input.hpp"

class Shape2D : public sf::Shape
{
  public:
    math::PolygonF polygon;

    Shape2D( math::PolygonF aPolygon ) : polygon { aPolygon } {}

    unsigned int get_point_count() const
    {
        return polygon.get_number_of_points();
    };
    math::Vector2F get_point( unsigned int index ) const
    {
        return polygon[index];
    };

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

class StaticEntity2D : public TransformableComponent2D
{
  protected:
    Shape2D m_shape;

  public:
    explicit StaticEntity2D( math::PolygonF polygon );
    ~StaticEntity2D() = default;

    math::PointF get_position() const;
    math::PolygonF get_polygon( bool getSize = true ) const;
    Shape2D & get_shape() { return m_shape; }

    void set_polygon( math::PolygonF polygon );

    bool is_intersected_by( StaticEntity2D const & otherEntity ) const;

  protected:
    void render( Render & render ) const override
    {
        render.get_target().draw( m_shape, render.get_state() );
    }
};

/// @brief Moveable Entity
class Entity2D : public StaticEntity2D
{
    std::vector< StaticEntity2D > const & m_collisionMap;
    View const & m_view;
    input::S_KeyboardMove m_movementKey;
    math::Vector2F m_speed;

  public:
    Entity2D( math::PolygonF quadrangle,
              std::vector< StaticEntity2D > const & collisionMap,
              View const & view, input::S_KeyboardMove movementKey );

    math::Vector2F get_speed() const;

    void set_speed( float speed );
    void set_speed( math::Vector2F speed );

    bool is_collision_detected() const;

    void update_extra( float deltaTime ) override;
};

namespace customize
{
    static void circle_shape( std::string windowName,
                              sf::CircleShape & circleShape )
    {
        if ( ImGui::Begin( windowName.c_str() ) )
        {
            sf::Color background { circleShape.getFillColor() };
            sf::Color outline { circleShape.getOutlineColor() };
            float outlineThickness { circleShape.getOutlineThickness() };
            float radius { circleShape.getRadius() };
            unsigned int numberOfPoint { static_cast< unsigned int >(
                circleShape.getPointCount() ) };

            ImGui::P_ColorEditor( "Background Color", background );
            ImGui::P_ColorEditor( "Outline Color", outline );
            ImGui::InputFloat( "Outline Thickness", &outlineThickness );
            ImGui::InputFloat( "Radius", &radius );
            ImGui::P_InputNumber( "Number of Point", numberOfPoint );

            circleShape.setFillColor( background );
            circleShape.setOutlineColor( outline );
            circleShape.setOutlineThickness( outlineThickness );
            circleShape.setRadius( radius );
            circleShape.setPointCount( numberOfPoint );
        }
        ImGui::End();
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

        if ( ImGui::Begin( "Customise Entity", &m_showCustomisation ) )
        {
            customize::circle_shape( "Customise Entity", m_pointShape );

            ImGui::Checkbox( "Edit Points ?", &m_editPoint );
        }
        ImGui::End();
    }

  private:
    void render( Render & render ) const override
    {
        StaticEntity2D::render( render );

        sf::CircleShape pointShape { m_pointShape };
        for ( math::PointF const & point : m_shape.polygon.get_points() )
        {
            pointShape.setPosition( point.x, point.y );
            render.get_target().draw( pointShape, render.get_state() );
        }
    }
};