#pragma once

#include <functional>  // for reference_wrapper
#include <vector>      // for vector

#include "application/contexts/render_context.hpp"  // for RenderContext
#include "application/contexts/update_context.hpp"  // for UpdateContext

class View;

class Component;
// check if T is derived from Component
template< typename T > concept cComponent = std::derived_from< T, Component >;

class Component
{
    // List of childs components
    std::vector< Component * > m_childs;

    // optionnal references so we use a pointer
    View const * m_view;

  protected:
    Component();

  public:
    virtual ~Component() = default;

    Component( Component const & component ) noexcept;
    Component( Component && component ) noexcept;
    Component & operator= ( Component const & component ) noexcept;
    Component & operator= ( Component && component ) noexcept;

  protected:
    void add_child ( Component & component );
    void add_child ( Component & component, View const & view );
    void add_child ( Component * component );

    void remove_child ( Component * component );
    void remove_child ( Component & component );

  public:
    std::vector< Component const * > get_childs () const;
    std::vector< Component * >       get_childs ();

    // update the component
    virtual void update_all ( UpdateContext & context );
    // draw the component to the render
    virtual void render_all ( RenderContext & context ) const;

    void         set_view ( View const & view );
    View const & get_view () const;

  private:
    // custom update - called before children update
    virtual void update_before ( UpdateContext & context );
    // custom update - order doesn't matter
    virtual void update ( UpdateContext & context );
    // custom update - called after children update
    virtual void update_after ( UpdateContext & context );

    // custom render - called before children render
    virtual void render_before ( RenderContext & context ) const;
    // custom render - order doesn't matter
    virtual void render ( RenderContext & context ) const;
    // custom render - called after children render
    virtual void render_after ( RenderContext & context ) const;
};
