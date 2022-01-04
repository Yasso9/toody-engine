#pragma once

#include <project/tools/types.hpp>

namespace InputInitialization
{
    KeyboardInputMap keyboard( StateName const & stateName );

    MouseInputMap mouse_button( StateName const & stateName );
} // namespace InputInitialization
