#pragma once

#include "states/state.hpp"

namespace InputInitialization
{
    T_KeyboardInputMap keyboard( State::E_List const & stateName );

    T_MouseInputMap mouse_button( State::E_List const & stateName );
} // namespace InputInitialization
