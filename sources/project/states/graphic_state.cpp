#include "graphic_state.hpp"

#include <cmath>
#include <iostream>

GraphicState::GraphicState( std::shared_ptr<sf::RenderWindow> window,
                            Ressources const & ressources )
  : State( window, ressources, State::E_List::Graphics )
{
    gl::initialize();

    this->init_shape();
}

void GraphicState::update()
{
    this->update_keyboard();
    this->update_mouse();

    this->update_camera();
}

void GraphicState::render()
{
    gl::clear_window();

    this->m_shape.draw();
}

void GraphicState::init_shape()
{
    std::vector<float> vertices {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f
    };
    unsigned int const numberOfDataPerAttribute { 5u };
    std::vector<unsigned int> indices {};

    this->m_shape.create( vertices, indices, numberOfDataPerAttribute );
}

void GraphicState::mouse_scroll( float const & deltaScroll )
{
    float scrollSpeed { 5.f };

    if ( deltaScroll < 0.1f )
    {
        scrollSpeed = -scrollSpeed;
    }
    else // positiv scroll
    {}

    this->m_camera.zoom( scrollSpeed, this->m_deltaTime );
}

void GraphicState::update_keyboard()
{
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )
    {
        this->m_camera.move( Camera::E_Movement::Forward, this->m_deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
    {
        this->m_camera.move( Camera::E_Movement::Backward, this->m_deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) )
    {
        this->m_camera.move( Camera::E_Movement::Left, this->m_deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
    {
        this->m_camera.move( Camera::E_Movement::Right, this->m_deltaTime );
    }

    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) )
    {
        this->m_camera.rotate( { 0.f, 1.f }, this->m_deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) )
    {
        this->m_camera.rotate( { 0.f, -1.f }, this->m_deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Left ) )
    {
        this->m_camera.rotate( { -1.f, 0.f }, this->m_deltaTime );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Right ) )
    {
        this->m_camera.rotate( { 1.f, 0.f }, this->m_deltaTime );
    }
}

void GraphicState::update_mouse()
{
    sf::Vector2i const i_middlePosition {
        this->m_window.get()->getPosition()
        + static_cast<sf::Vector2i>( this->m_window.get()->getSize() / 2u )
    };
    sf::Vector2f const f_middlePosition { static_cast<sf::Vector2f>(
        i_middlePosition ) };

    std::cout << "screen resolution" << this->m_window.get()->getSize()
              << std::endl;
    std::cout << "middlePosition" << f_middlePosition << std::endl;

    sf::Vector2f const currentMousePosition { sf::Mouse::getPosition() };

    sf::Vector2f offset {};
    offset.x = currentMousePosition.x - f_middlePosition.x;
    offset.y = f_middlePosition.y - currentMousePosition.y;

    // TYPO faire une fonction "set_mouse_relativ_position"
    // Reset Mouse Position
    sf::Mouse::setPosition( static_cast<sf::Vector2i>( i_middlePosition ) );

    float const sensitivity = 1.f;
    offset *= sensitivity;

    this->m_camera.rotate( offset, this->m_deltaTime );
}

void GraphicState::update_camera()
{
    gl::SpaceMatrix spaceMatrix {};

    // space transformation
    {
        float const fieldOfView { glm::radians( this->m_camera.get_zoom() ) };
        float const screenRatio { static_cast<float>(
            this->m_window.get()->getSize().x
            / this->m_window.get()->getSize().y ) };
        float const nearDistanceFromCamera { 0.1f };
        float const farDistanceFromCamera { 100.f };

        spaceMatrix.projection = glm::perspective( fieldOfView,
                                                   screenRatio,
                                                   nearDistanceFromCamera,
                                                   farDistanceFromCamera );
    }

    // camera/view transformation
    {
        spaceMatrix.view = this->m_camera.get_view_matrix();
    }

    // object transformation
    {
        glm::mat4 const translation {
            glm::translate( glm::mat4 { 1.f }, glm::vec3 { 0.f, 0.f, 0.f } )
        };
        glm::mat4 const rotation { glm::rotate( glm::mat4 { 1.f },
                                                glm::radians( 0.f ),
                                                glm::vec3 { 1.f, 1.f, 1.f } ) };
        glm::mat4 const scale { glm::scale( glm::mat4 { 1.f },
                                            glm::vec3 { 1.f, 1.f, 1.f } ) };
        spaceMatrix.model = translation * rotation * scale;
    }

    this->m_shape.update( spaceMatrix );
}