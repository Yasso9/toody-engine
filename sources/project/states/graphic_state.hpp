#pragma once

#include "states/state.hpp"

class GraphicState final : public State
{
  public:
    GraphicState( std::shared_ptr<sf::RenderWindow> window,
                  Ressources const & ressources, Settings const & settings );
    virtual ~GraphicState();

    void update() override;
    void render() override;

  private:
    unsigned int m_vertexArrayObject;
    unsigned int m_vertexBufferObject;
    unsigned int m_elementBufferObject;

    unsigned int m_shaderProgram;
};