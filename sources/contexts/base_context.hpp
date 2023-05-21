#pragma once

#include "interface/window.hpp"  // for Window

class BaseContext
{
  public:
    Window window;
    BaseContext();
    virtual ~BaseContext() = default;
};
