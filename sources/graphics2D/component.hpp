#pragma once

#include <concepts>
#include <memory>
#include <optional>

#include "graphics2D/sfml.hpp"
#include "main/render.hpp"

class Window;

class Component;

/// @brief check if the Type is derived from Component
template < typename Type >
concept C_IsComponent = std::derived_from< Type, Component >;

class Component
{
  private:
    std::vector< Component * > m_childs;

  protected:
    Component() : m_childs {} {};

  public:
    virtual ~Component() = default;

  protected:
    template < C_IsComponent ComponentClass >
    void add_child( ComponentClass & component );
    template < C_IsComponent ComponentClass >
    void add_childs( std::vector< ComponentClass > & components );

  public:
    /// @brief update the component
    virtual void update( float deltaTime ) final;
    /// @brief draw the component to the render
    virtual void render_all( Render & render ) const final;

  protected:

  private:
    /// @brief custom update
    virtual void update_before( float deltaTime );
    /// @brief custom update
    virtual void update_after( float deltaTime );
    /// @brief custom render
    virtual void render_before( Render & render ) const;
    /// @brief custom render
    virtual void render_after( Render & render ) const;
};

// A voir si on garde tous ça ?
class Component2D : public Component
{
  protected:
    Component2D()          = default;
    virtual ~Component2D() = default;
};
class Component3D : public Component
{
  protected:
    Component3D()          = default;
    virtual ~Component3D() = default;
};
class TransformableComponent2D : public Component2D,
                                 public sf::Transformable
{
  protected:
    TransformableComponent2D()          = default;
    virtual ~TransformableComponent2D() = default;
};

#include "component.tpp"