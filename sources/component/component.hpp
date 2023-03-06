#pragma once

#include <optional>
#include <vector>  // for vector

#include <SFML/Graphics/Transformable.hpp>  // for Transformable

#include "graphics2D/view.hpp"
#include "main/render.hpp"

class Component;

/// @brief check if the Type is derived from Component
template< typename Type >
concept C_IsComponent = std::derived_from< Type, Component >;

class Component
{
  private:
    /// @brief Vector of pointer to child component
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
    /// @brief update the component. Must not be called manually if it's a child
    virtual void update_all ( float deltaTime ) final;
    /// @brief draw the component to the render. Must not be called manually if
    /// it's a child
    virtual void render_all ( Render & render ) const;

    void set_view ( View const & view );

  private:
    /// @brief custom update - called before children update
    virtual void update_before ( float deltaTime );
    /// @brief custom update - order doesn't matter
    virtual void update ( float deltaTime );
    /// @brief custom update - called after children update
    virtual void update_after ( float deltaTime );

    /// @brief custom render - called before children render
    virtual void render_before ( Render & render ) const;
    /// @brief custom render - order doesn't matter
    virtual void render ( Render & render ) const;
    /// @brief custom render - called after children render
    virtual void render_after ( Render & render ) const;
};

#include "component.tpp"