#pragma once

#include "graphics/openGL.hpp"

unsigned int TextureFromFile( const char * path,
                              const std::string & directory );

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wcast-qual"
// #pragma GCC diagnostic ignored "-Wsign-conversion"
// #pragma GCC diagnostic ignored "-Wold-style-cast"
// #pragma GCC diagnostic ignored "-Wimplicit-int-conversion"
// #pragma GCC diagnostic ignored "-Wcast-align"
// #define STB_IMAGE_IMPLEMENTATION
// #include <LIBS/stb_image.h>
// #pragma GCC diagnostic pop

// #include "graphics/openGL.hpp"

// class Texture final
// {
//   public:
//     Texture()          = default;
//     virtual ~Texture() = default;

//     void load();
//     void bind() const;

//   private:
//     // unsigned int m_id;
// };