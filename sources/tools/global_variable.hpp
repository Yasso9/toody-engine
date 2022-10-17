#pragma once

#include "maths/maths.hpp"

constexpr unsigned int const COLOR_RANGE_U { 255u };
constexpr float const COLOR_RANGE_F { static_cast< float >( COLOR_RANGE_U ) };

constexpr unsigned int const TILE_PIXEL_SIZE_U { 32u };
constexpr float const        TILE_PIXEL_SIZE {
    static_cast< float >( TILE_PIXEL_SIZE_U ) };
constexpr int const TILE_PIXEL_SIZE_I {
    static_cast< int >( TILE_PIXEL_SIZE_U ) };

constexpr math::Vector2F const TILE_PIXEL_SIZE_VECTOR {
    TILE_PIXEL_SIZE, TILE_PIXEL_SIZE };