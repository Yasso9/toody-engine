#include "editor_state.hpp"

#include "tools/string.hpp"

#include <cassert>

EditorState::EditorState( std::shared_ptr<sf::RenderWindow> window,
                          Ressources const & ressources )
  : State( window, ressources, State::E_List::Editor ),
    m_tilemap( this->m_ressources.textures.at( E_TextureKey::Tileset ) ),
    m_tileset( this->m_ressources.textures.at( E_TextureKey::Tileset ) ),
    m_buttons( this->m_ressources.fonts.at( E_FontKey::Arial ) )
{
    this->m_type = Type::Normal;

    this->init_map();
    this->init_selection_rect();

    this->m_buttons.set_strings( { "Normal"s, "Selection"s } );
    this->m_buttons.setPosition( 0.f, 0.f );
}

void EditorState::init_map()
{
    this->m_tilemap.setPosition(
        ( this->m_settings.get_window_size_f() - this->m_tilemap.get_size() )
        / 2.f );

    // Init view
    this->m_view.setCenter( this->m_settings.get_window_size_f() / 2.f );
    this->m_view.setSize( this->m_settings.get_window_size_f() );

    this->m_tileset.setPosition( this->m_settings.get_window_size_f() / 5.f );
    // TYPO la taille ne doit pas être aberrante
    this->m_tileset.set_size( this->m_settings.get_window_size_f() / 3.f );
}

// Apres
void EditorState::init_selection_rect()
{
    // this->m_selectionRect.setPosition( 0.f, 0.f );
    // this->m_selectionRect.setSize( sf::Vector2f( 0.f, 0.f ) );
    // this->m_selectionRect.setFillColor( sf::Color( 255, 230, 90, 128 ) );
}

void EditorState::mouse_scroll( float const & deltaScroll )
{
    float const scaleValue { 1.f + ( deltaScroll / 4.f ) };
    this->m_tilemap.scale( scaleValue, scaleValue );
}

void EditorState::keyboard_pressed( sf::Event event )
{
    if ( event.key.code == sf::Keyboard::Num1 )
    {
        this->m_tilemap.set_depth( 0 );
    }
    else if ( event.key.code == sf::Keyboard::Num2 )
    {
        this->m_tilemap.set_depth( 1 );
    }
    else if ( event.key.code == sf::Keyboard::C )
    {
        // TYPO mettre le centre de la vue et non pas la taille de la fenetre / 2
        this->m_tilemap.setPosition( this->m_settings.get_window_size_f()
                                     / 2.f );
    }
    else if ( event.key.code == sf::Keyboard::Space )
    {
        this->m_tileset.switch_print();
    }
    else if ( event.key.code == sf::Keyboard::Enter )
    {
        this->m_tilemap.save();
    }
}

void EditorState::update_normal_mode()
{
    this->m_tileset.update(
        static_cast<sf::Vector2f>( sf::Mouse::getPosition() ),
        sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) );

    this->m_tilemap.update(
        static_cast<sf::Vector2f>( sf::Mouse::getPosition() ),
        static_cast<unsigned int>( this->m_tileset.get_selected_tile() ),
        sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) );
}

void EditorState::update_selection_mode() {}

void EditorState::update_colision_mode() {}

void EditorState::update()
{
    // Movement de la mapView par raport a l'ecran,
    // deplacer la vue revient a deplacer la mapView
    // On ne met pas de else if pour pouvoir avoir un mouvement multiple
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )
    {
        this->m_tilemap.move( 0.f, 5.f );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
    {
        this->m_tilemap.move( 0.f, -5.f );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) )
    {
        this->m_tilemap.move( 5.f, 0.f );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
    {
        this->m_tilemap.move( -5.f, 0.f );
    }

    // Always check if the buttons have been selected or pressed
    int const buttonNumberPressed { this->m_buttons.update(
        static_cast<sf::Vector2f>( sf::Mouse::getPosition() ),
        sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) ) };

    if ( buttonNumberPressed != -1 )
    {
        this->m_type = static_cast<Type>( buttonNumberPressed );
    }

    switch ( this->m_type )
    {
    case Type::Normal :
        this->update_normal_mode();
        break;
    case Type::Selection :
        this->update_selection_mode();
        break;
    case Type::Colision :
        this->update_colision_mode();
        break;
    default :
        assert( false && "Editor Type does not exist" );
        break;
    }
}

void EditorState::render()
{
    m_window->setView( this->m_view );

    m_window->draw( this->m_tilemap );
    m_window->draw( this->m_tileset );

    m_window->draw( this->m_buttons );
}