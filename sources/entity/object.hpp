#pragma once

#include "graphics2D/component.hpp"
#include "graphics2D/shape.hpp"
#include "graphics2D/view.hpp"
#include "input/input.hpp"

class StaticEntity2D : public Component2D,
                       public Shape2D
{
  public:
    explicit StaticEntity2D( math::PolygonF polygon );
    ~StaticEntity2D() = default;

    bool is_intersected_by( StaticEntity2D const & otherEntity ) const;

  protected:
    void render_before( Render & render ) const override;
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

    virtual void update_before( float deltaTime ) override;
};