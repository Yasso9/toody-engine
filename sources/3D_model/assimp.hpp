#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#if defined( __clang__ )
    #pragma GCC diagnostic ignored "-Wmicrosoft-enum-value"
    #pragma GCC diagnostic ignored "-Wimplicit-float-conversion"
#endif

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#pragma GCC diagnostic pop