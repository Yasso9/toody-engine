#pragma once

#include <memory>  // for shared_ptr

#include "contexts/base_context.hpp"  // for BaseContext
#include "interface/inputs.hpp"       // for Inputs
#include "interface/window.hpp"       // for Window

class State;
enum class StateList;

class UpdateContext : virtual public BaseContext
{
  protected:
    std::shared_ptr< State > m_state;

  public:
    Inputs inputs;
    float  deltaTime;
    bool   shouldRun;

    UpdateContext();
    virtual ~UpdateContext() = default;

    void transition_to ( std::shared_ptr< State > state );
    void transition_to ( StateList state );

  protected:
    void clear ();
};
