#include "editor_state.hpp"

#include "tools/string.hpp"

#include <cassert>

EditorState::EditorState( std::shared_ptr<sf::RenderWindow> window,
                          Ressources const & ressources,
                          Settings const & settings )
  : State( window, ressources, settings, State::E_List::Editor ),
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

void EditorState::handle_mouse_wheel_up()
{
    this->m_tilemap.scale( 1.2f, 1.2f );
}

void EditorState::handle_mouse_wheel_down()
{
    this->m_tilemap.scale( 0.8f, 0.8f );
}

void EditorState::handle_keyboard_press( std::string const & input )
{
    if ( input == "MainMenu"s )
    {
        this->m_stateName = State::E_List::MainMenu;
    }
    else if ( input == "Profondeur1"s )
    {
        this->m_tilemap.set_depth( 0 );
    }
    else if ( input == "Profondeur2"s )
    {
        this->m_tilemap.set_depth( 1 );
    }
    else if ( input == "Center"s )
    {
        // TYPO mettre le centre de la vue et non pas la taille de la fenetre / 2
        this->m_tilemap.setPosition( this->m_settings.get_window_size_f()
                                     / 2.f );
    }
    else if ( input == "PrintOrRemoveSpriteSheet"s )
    {
        this->m_tileset.switch_print();
    }
    else if ( input == "Save"s )
    {
        this->m_tilemap.save();
    }
}

void EditorState::handle_current_input()
{
    // Movement de la mapView par raport a l'ecran,
    // deplacer la vue revient a deplacer la mapView
    // On ne met pas de else if pour pouvoir avoir un mouvement multiple
    if ( this->m_keyboard.at( "TilemapUp"s ).second )
    {
        this->m_tilemap.move( 0.f, 5.f );
    }
    if ( this->m_keyboard.at( "TilemapDown"s ).second )
    {
        this->m_tilemap.move( 0.f, -5.f );
    }
    if ( this->m_keyboard.at( "TilemapLeft"s ).second )
    {
        this->m_tilemap.move( 5.f, 0.f );
    }
    if ( this->m_keyboard.at( "TilemapRight"s ).second )
    {
        this->m_tilemap.move( -5.f, 0.f );
    }

    // Always check if the buttons have been selected or pressed
    int const buttonNumberPressed { this->m_buttons.update(
        this->m_mousePosition.get_overall(),
        this->m_mouseButton.at( "Action"s ).second ) };

    if ( buttonNumberPressed != -1 )
    {
        this->m_type = static_cast<Type>( buttonNumberPressed );
    }
}

void EditorState::update_normal_mode()
{
    this->m_tileset.update( this->m_mousePosition.get_overall(),
                            this->m_mouseButton.at( "Action"s ).second );

    this->m_tilemap.update(
        this->m_mousePosition.get_overall(),
        // TYPO voir pour enlever ce static cast
        static_cast<unsigned int>( this->m_tileset.get_selected_tile() ),
        this->m_mouseButton.at( "Action"s ).second );
}

void EditorState::update_selection_mode() {}

void EditorState::update_colision_mode() {}

void EditorState::update( float const & /* deltaTime */ )
{
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