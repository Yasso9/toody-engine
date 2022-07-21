#pragma once

#include <concepts>
#include <memory>
#include <optional>

#include "graphics2D/sfml.hpp"
#include "main/render.hpp"

class Window;

class BaseComponent;
class Component2D;
class Component3D;

/// @brief check if the Type is derived from BaseComponent
template < typename Type >
concept C_IsComponent = std::derived_from< Type, BaseComponent >;
/// @brief check if the Type is derived from Component2D
template < typename Type >
concept C_IsComponent2D = std::derived_from< Type, Component2D >;
/// @brief check if the Type is derived from Component3D
template < typename Type >
concept C_IsComponent3D = std::derived_from< Type, Component3D >;

class BaseComponent
{
    friend class Component2D;
    friend class Component3D;

  protected:
    std::vector< std::shared_ptr< Component2D > > m_childs2D;
    std::vector< std::shared_ptr< Component3D > > m_childs3D;
    /// @todo faire un get_child quir éupère le type de la classe hérité

    BaseComponent() : m_childs2D {}, m_childs3D {} {};
    virtual ~BaseComponent() = default;

    template < C_IsComponent2D ComponentClass >
    void add_child( ComponentClass & component );
    template < C_IsComponent3D ComponentClass >
    void add_child( ComponentClass & component );
    template < C_IsComponent ComponentClass >
    void add_childs( std::vector< ComponentClass > & components );

  public:
    virtual void update( float deltaTime ) final;
    virtual void render_all( Render & render ) const final;

  private:
    virtual void update_extra( float deltaTime );
    virtual void render_before( Render & render ) const;
    virtual void render( Render & render ) const;
    virtual void render_after( Render & render ) const;
};

/// @brief OpenGL Component
class Component3D : public BaseComponent
{
  protected:
    Component3D() = default;
};

/// @brief SFML Component
class Component2D : public BaseComponent,
                    public sf::Drawable
{
    friend class TransformableComponent2D;

  protected:
    Component2D() = default;

    /// @todo trouver un moyen pour que cela ne soit pas virtual
    /// @brief do not overload this unless if you are sure about what to do
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