#pragma once

class TileMapEditor
{
  public:
    TileMapEditor();
    ~TileMapEditor() = default;

    void update();

  private:
    bool m_isGridEnabled;
    float m_gridColorTable[4];
};