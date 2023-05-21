#pragma once

#include "contexts/render_context.hpp"  // for RenderContext
#include "contexts/update_context.hpp"  // for UpdateContext
#include "states/state.hpp"             // for State, State::E_List

class GameContext final : public UpdateContext,
                          public RenderContext
{
  public:
    GameContext();
    virtual ~GameContext();

    void update ( float aDeltaTime );
};
