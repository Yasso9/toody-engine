#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundef"
#if defined( __GNUC__ )
    #pragma GCC diagnostic ignored "-Wold-style-cast"
    #pragma GCC diagnostic ignored "-Weffc++"
    #pragma GCC diagnostic error "-Wundef"
#endif

#include "IMGUI/imgui-SFML.h"
#include "IMGUI/imgui.h"

#pragma GCC diagnostic pop