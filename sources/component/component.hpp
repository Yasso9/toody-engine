#pragma once

#include <memory>  // for shared_ptr
#include <optional>
#include <vector>  // for vector

#include <SFML/Graphics/Transformable.hpp>  // for Transformable

#include "component/contexts.hpp"
#include "graphics2D/view.hpp"
#include "main/render.hpp"

class Component;

/// @brief check if the T is derived from Component
template< typename T >
concept C_IsComponent = std::derived_from< T, Component >;

class Component
{
  private:
    /// @brief Vector of pointer to child component
    std::vector< Component * > m_childs;
    /// @todo have an optionnal view
    View const *               m_view;

  protected:
    Component();

  public:
    virtual ~Component() = default;

    Component( Component const & component ) noexcept;
    Component( Component && component ) noexcept;
    Component & operator= ( Component const & component ) noexcept;
    Component & operator= ( Component && component ) noexcept;

  protected:
    template< C_IsComponent ComponentClass >
    void add_child ( ComponentClass * component );
    template< C_IsComponent ComponentClass >
    void add_child ( ComponentClass & component );
    /// @brief Add a child to the component
    /// @tparam ComponentClass Type of the component. only Class inherited from
    /// Component are accepted
    /// @param component Child of the component to add. Will call update and
    /// render automatically
    /// @param view View that the component will have when it's drawn
    /// @todo voir si c'est utile d'ajouter view
    template< C_IsComponent ComponentClass >
    void add_child ( ComponentClass & component, View const & view );
    template< C_IsComponent ComponentClass >
    void add_childs ( std::vector< ComponentClass > & components );

    template< C_IsComponent ComponentClass >
    void remove_child ( ComponentClass * component );
    template< C_IsComponent ComponentClass >
    void remove_child ( ComponentClass & component );

  public:
    std::vector< Component const * > get_childs () const;
    std::vector< Component * >       get_childs ();

    /// @brief update the component. Must not be called manually if it's a child
    virtual void update_all ( UpdateContext context );
    /// @brief draw the component to the render. Must not be called manually if
    /// it's a child
    virtual void render_all ( RenderContext context ) const;

    void set_view ( View const & view );

  private:
    /// @brief custom update - called before children update
    virtual void update_before ( UpdateContext context );
    /// @brief custom update - order doesn't matter
    virtual void update ( UpdateContext context );
    /// @brief custom update - called after children update
    virtual void update_after ( UpdateContext context );

    /// @brief custom render - called before children render
    virtual void render_before ( RenderContext context ) const;
    /// @brief custom render - order doesn't matter
    virtual void render ( RenderContext context ) const;
    /// @brief custom render - called after children render
    virtual void render_after ( RenderContext context ) const;
};

#include "component.tpp"