#pragma once

#include <memory>  // for shared_ptr

#include "component/component.hpp"
#include "states/state.hpp"  // for State, State::E_List

class Game : public Component
{
    std::shared_ptr< State > m_state;
    bool                     m_shouldRun;

  public:
    Game();
    virtual ~Game();

    virtual void update_all ( UpdateContext context ) override;
    virtual void render_all ( RenderContext & context ) const override;

    void update_inputs ( Window & window );

    bool should_run () const;

  private:
    void change_state ( State::E_List const & newState );
};