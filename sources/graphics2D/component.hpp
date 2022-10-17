#pragma once

#include <optional>
#include <vector>  // for vector

#include <SFML/Graphics/Transformable.hpp>  // for Transformable

#include "graphics2D/view.hpp"

class Component;
class Render;

/// @brief check if the Type is derived from Component
template< typename Type >
concept C_IsComponent = std::derived_from< Type, Component >;

class Component
{
  private:
    std::vector< Component * > m_childs;
    /// @todo have an optionnal view
    View const *               m_view;

  protected:
    // Virtual Class - Can only be called by child
    Component() : m_childs {}, m_view { nullptr } {};

  public:
    virtual ~Component() = default;

    Component( Component const & component ) noexcept;
    Component( Component && component ) noexcept;
    Component & operator= ( Component const & component ) noexcept;
    Component & operator= ( Component && component ) noexcept;

  protected:
    template< C_IsComponent ComponentClass >
    void add_child ( ComponentClass & component );
    /// @brief Add a child to the component
    /// @tparam ComponentClass Type of the component. only Class inherited from
    /// Component are accepted
    /// @param component Child of the component to add. Will call update and
    /// render automatically
    /// @param view View that the component will have when it's drawn
    template< C_IsComponent ComponentClass >
    void add_child ( ComponentClass & component, View const & view );
    template< C_IsComponent ComponentClass >
    void add_childs ( std::vector< ComponentClass > & components );

  public:
    /// @brief update the component
    virtual void update ( float deltaTime ) final;
    /// @brief draw the component to the render
    virtual void render_all ( Render & render ) const final;

    void set_view ( View const & view );

  private:
    /// @brief custom update - is the first function called
    /// in the update procedure
    virtual void update_before ( float deltaTime );
    /// @brief custom update - is the last function called
    /// in the update procedure
    virtual void update_after ( float deltaTime );
    /// @brief custom render - is the first function called
    /// in the render procedure
    virtual void render_before ( Render & render ) const;
    /// @brief custom render - is the last function called
    /// in the render procedure
    virtual void render_after ( Render & render ) const;
};

/// @todo A voir si on garde tous ça ?
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
