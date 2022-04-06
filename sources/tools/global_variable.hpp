#pragma once

constexpr float const g_framerate { 60.f };

// TYPO il faut essayer au maximum d'utiliser les g_tileSize_u au lieu des g_tileSize_f
/// @brief unsigned integer size of the a tile square in pixel
constexpr unsigned int const g_tileSize_u { 32u };
/// @brief float size of the a tile square in pixel
constexpr float const g_tileSize_f { static_cast< float >( ::g_tileSize_u ) };

// TYPO a enlever, peut se calculer automatiquement en comptant le nb d'instance
// d'enfant de State
constexpr unsigned int const g_numberOfState { 3 };