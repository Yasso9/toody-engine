#pragma once

class TileSelector
{
  public:
    TileSelector();
    virtual ~TileSelector() = default;

    void update();

  private:
    bool m_isGridEnabled;
    float m_gridColorTable[4];
};