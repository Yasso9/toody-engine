#pragma once

#include "application/contexts/render_context.hpp"  // for RenderContext
#include "application/contexts/update_context.hpp"  // for UpdateContext
#include "states/state.hpp"             // for State, StateList

class GameContext final : public UpdateContext,
                          public RenderContext
{
  public:
    GameContext();
    virtual ~GameContext();

    void update ( float aDeltaTime );
};
