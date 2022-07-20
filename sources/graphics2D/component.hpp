#pragma once

#include <concepts>
#include <memory>

#include "graphics2D/sfml.hpp"

class Component;

/// @brief check if the Type is derived from Component
template < typename Type >
concept C_Component = std::derived_from< Type, Component >;

class Component : public sf::Drawable
{
    std::vector< std::shared_ptr< Component > > m_childs {};

  public:
    void update( float deltaTime );

  protected:
    template < C_Component ComponentClass >
    void add_child( ComponentClass & component );
    template < C_Component ComponentClass >
    void add_childs( std::vector< ComponentClass > & components );

  private:
    friend class TransformableComponent;

    virtual void update_extra( float deltaTime );

    virtual void render_before( sf::RenderTarget & target,
                                sf::RenderStates states ) const;
    virtual void render( sf::RenderTarget & target,
                         sf::RenderStates states ) const;
    virtual void render_after( sf::RenderTarget & target,
                               sf::RenderStates states ) const;

    /// @todo trouver un moyen pour que cela ne soit pas virtual
    /// @brief do not overload this unless if you are sure about what to do
    virtual void draw( sf::RenderTarget & target,
                       sf::RenderStates states ) const override;
};

class TransformableComponent : public Component,
                               public sf::Transformable
{
  private:
    void draw( sf::RenderTarget & target,
               sf::RenderStates states ) const override
    {
        states.transform *= this->getTransform();

        Component::draw( target, states );
    }
};

#include "component.tpp"