#pragma once

#include <memory>  // for shared_ptr

#include "states/state.hpp"  // for State, State::E_List
#include "tools/traces.hpp"  // for Traces

class GameContext
{
    std::shared_ptr< State > m_state;
    bool                     m_shouldRun;

  public:
    GameContext();
    virtual ~GameContext();

    void transition_to ( std::shared_ptr< State > state );
    void transition_to ( State::E_List state );

    bool should_run () const;
    void should_run ( bool shouldRun );

    State &       get_state ();
    State const & get_state () const;
};