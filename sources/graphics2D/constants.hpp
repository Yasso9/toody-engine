#pragma once

#include "maths/maths.hpp"

constexpr unsigned int COLOR_RANGE_U { 255u };
constexpr float        COLOR_RANGE_F { static_cast< float >( COLOR_RANGE_U ) };

constexpr unsigned int TILE_PIXEL_SIZE_U { 32u };
constexpr float TILE_PIXEL_SIZE { static_cast< float >( TILE_PIXEL_SIZE_U ) };
constexpr int   TILE_PIXEL_SIZE_I { static_cast< int >( TILE_PIXEL_SIZE_U ) };

constexpr math::Vector2F TILE_PIXEL_SIZE_VECTOR { TILE_PIXEL_SIZE,
                                                  TILE_PIXEL_SIZE };