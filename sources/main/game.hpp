#pragma once

#include <memory>  // for shared_ptr

#include "component/component.hpp"
#include "states/state.hpp"  // for State, State::E_List

class Game : public Component
{
  private:
    std::shared_ptr< State > m_state;
    bool                     m_shouldRun;

  public:
    Game();
    virtual ~Game();

  private:
    virtual void update ( float deltaTime ) override;
    virtual void render ( Render & render ) const override;

  public:
    bool should_run () const;

  private:
    /// @brief change the state to a new state
    /// @param newState the new state to be printed
    void change_state ( State::E_List const & newState );
};