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
  public:
    enum class E_Type
    {
        OpenGL = 0,
        SFML,
    };

  private:
    E_Type m_type;
    std::vector< std::shared_ptr< Component > > m_childs;

  protected:
    Component( E_Type type ) : m_type( type ), m_childs {} {};
    virtual ~Component() = default;

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
    virtual void update_extra( float deltaTime );
    /// @brief custom render
    virtual void render_before( Render & render ) const;
    /// @brief custom render
    virtual void render( Render & render ) const;
    /// @brief custom render
    virtual void render_after( Render & render ) const;

  public:
    E_Type get_type() const;
};

class Component3D : public Component
{
  protected:
    Component3D() : Component { Component::E_Type::OpenGL } {}
    virtual ~Component3D() = default;
};

class Component2D : public Component,
                    public sf::Drawable
{
    friend class TransformableComponent2D;

  protected:
    Component2D() : Component { Component::E_Type::SFML } {}
    virtual ~Component2D() = default;

  private:
    virtual void draw( sf::RenderTarget & target,
                       sf::RenderStates states ) const override;
};

class TransformableComponent2D : public Component2D,
                                 public sf::Transformable
{
  protected:
    TransformableComponent2D() = default;

  private:
    virtual void draw( sf::RenderTarget & target,
                       sf::RenderStates states ) const override final
    {
        states.transform *= this->getTransform();

        Component2D::draw( target, states );
    }
};

#include "component.tpp"