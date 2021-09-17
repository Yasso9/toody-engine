#pragma once

#include <project/utility/utility.hpp>

namespace InputInitialization
{
    std::map<
        std::string const,
        std::pair<sf::Keyboard::Key const, bool>
    >keyboard( StateName const & stateName );

    std::map<
        std::string const,
        std::pair<sf::Mouse::Button const, bool>
    >mouse_button( StateName const & stateName );
}
