#pragma once

#include "tools/maths.hpp"

constexpr float const g_framerate { 60.f };

// TYPO il faut essayer au maximum d'utiliser les g_tileSize_u au lieu des g_tileSize_f
/// @brief unsigned integer size of the a tile square in pixel
constexpr unsigned int const g_tileSize_u { 32u };
/// @brief float size of the a tile square in pixel
constexpr float const g_tileSize_f { static_cast< float >( ::g_tileSize_u ) };

// TYPO a enlever, peut se calculer automatiquement en comptant le nb d'instance
// d'enfant de State
constexpr unsigned int const g_numberOfState { 3u };

constexpr unsigned int const COLOR_RANGE { 255u };
constexpr unsigned int const TILE_PIXEL_SIZE_U { 32u };
constexpr float const TILE_PIXEL_SIZE { static_cast< float >(
    TILE_PIXEL_SIZE_U ) };
constexpr math::Vector2D const TILE_PIXEL_SIZE_VECTOR { TILE_PIXEL_SIZE,
                                                        TILE_PIXEL_SIZE };