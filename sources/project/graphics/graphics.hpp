#pragma once

#include <glad/glad.h>
#include <SFML/OpenGL.hpp>

class Shaders final
{
  public:
    Shaders();
    virtual ~Shaders() = default;

  private:
};

namespace OpenGL
{
    void graphics();
}