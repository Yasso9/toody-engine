#include "editor_state.hpp"

EditorState::EditorState( Ressources const & ressources,
                          Settings const & settings )
  : State( ressources, settings, StateName::Editor ),
    m_tilemap( this->m_ressources.textures.at( TextureKey::Tileset ) ),
    m_tileset( this->m_ressources.textures.at( TextureKey::Tileset ) ),
    m_buttons( this->m_ressources.fonts.at( FontKey::Arial ),
               { "Normal", "Selection" } ),
    m_debugInformations( this->m_ressources.fonts.at( FontKey::Arial ) )
{
    this->m_tile = 0;
    this->m_mode = Mode::Normal;

    this->init_map();
    this->init_selection_rect();
}

void EditorState::init_map()
{
    this->m_tilemap.setPosition(
        ( this->m_settings.get_window_size_f() - this->m_tilemap.get_size() )
        / 2.f );

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
    if ( input == "MainMenu" )
    {
        this->m_stateName = StateName::MainMenu;
    }
    else if ( input == "Profondeur1" )
    {
        this->m_tilemap.set_actual_depth( 0 );
    }
    else if ( input == "Profondeur2" )
    {
        this->m_tilemap.set_actual_depth( 1 );
    }
    else if ( input == "Center" )
    {
        // TYPO mettre le centre de la vue et non pas la taille de la fenetre / 2
        this->m_tilemap.setPosition( this->m_settings.get_window_size_f()
                                     / 2.f );
    }
    else if ( input == "PrintOrRemoveSpriteSheet" )
    {
        this->m_tileset.switch_print();
    }
    else if ( input == "Save" )
    {
        this->m_tilemap.save();
    }
}

void EditorState::handle_current_input()
{
    // Movement de la mapView par raport a l'ecran,
    // deplacer la vue revient a deplacer la mapView
    // On ne met pas de else if pour pouvoir avoir un mouvement multiple
    if ( this->m_keyboard.at( "TilemapUp" ).second )
    {
        this->m_tilemap.move( 0.f, 5.f );
    }
    if ( this->m_keyboard.at( "TilemapDown" ).second )
    {
        this->m_tilemap.move( 0.f, -5.f );
    }
    if ( this->m_keyboard.at( "TilemapLeft" ).second )
    {
        this->m_tilemap.move( 5.f, 0.f );
    }
    if ( this->m_keyboard.at( "TilemapRight" ).second )
    {
        this->m_tilemap.move( -5.f, 0.f );
    }

    // Always check if the buttons have been selected or pressed
    int const buttonNumberPressed { this->m_buttons.update(
        this->m_mousePosition.get_overall(),
        this->m_mouseButton.at( "Action"s ).second ) };

    if ( buttonNumberPressed != -1 )
    {
        this->m_mode = static_cast<Mode>( buttonNumberPressed );
    }
}

void EditorState::update_normal_mode()
{
    this->m_tileset.update( this->m_mousePosition.get_overall(),
                            this->m_tile,
                            this->m_mouseButton.at( "Action" ).second );

    this->m_tilemap.update( this->m_mousePosition.get_overall(),
                            this->m_tile,
                            this->m_mouseButton.at( "Action" ).second );
}

void EditorState::update_selection_mode()
{
    if ( this->m_mouseButton.at( "Action" ).second )
    {
        // this->update_selection(
        //     to_map_view( this->m_mousePosition.get_press()
        //                  + sf::Vector2f( 16.f, 16.f ) ),
        //     to_map_view( this->m_mousePosition.get_overall()
        //                  + sf::Vector2f( SQUARE, SQUARE ) ) );
    }
}

void EditorState::update_colision_mode() {}

void EditorState::update()
{
    // Mise a jour des actions en fonction du mode
    if ( this->m_mode == Mode::Normal )
    {
        this->update_normal_mode();
    }
    else if ( this->m_mode == Mode::Selection )
    {
        this->update_selection_mode();
    }
    else if ( this->m_mode == Mode::Colision )
    {
        this->update_colision_mode();
    }

    sf::Vector2f const windowSize { this->m_settings.get_window_size_f() };
    // Mise a jour des informations
    this->m_debugInformations.update(
        sf::Vector2f( 0.8f * windowSize.x, 0.1f * windowSize.y ),

        { "Depth : "s + std::to_string( this->m_tilemap.get_actual_depth() ),
          "Mode : "s + std::to_string( static_cast<int>( this->m_mode ) ),
          "Mouse : "s + std::to_string( this->m_mousePosition.get_overall().x )
              + ", "s + std::to_string( this->m_mousePosition.get_overall().y ),
          "Mouse Press : "s
              + std::to_string( this->m_mousePosition.get_press().x ) + ", "
              + std::to_string( this->m_mousePosition.get_press().y ) } );
}

void EditorState::render( sf::RenderWindow & target )
{
    target.setView( this->m_view );

    target.draw( this->m_tilemap );
    target.draw( this->m_tileset );

    target.draw( this->m_buttons );
    target.draw( this->m_debugInformations );
}

// Apres (selection copy paste)
// void EditorState::copy_selection()
// {
//     sf::Vector2f tableBeginPos {
//     to_tile_position(this->m_selectionRect.getPosition() -
//     this->m_tileMapBegin) }; sf::Vector2f tableEndPos {
//     to_tile_position(this->m_selectionRect.getSize() -
//     this->m_selectionRect.getPosition() - this->m_tileMapBegin) };
//     this->tableSelection.clear();
//     for(auto j { tableBeginPos.y }; j <= tableEndPos.y; ++j)
//     {
//         for(auto i { tableBeginPos.x }; i <= tableEndPos.x; ++i)
//         {
//             this->tableSelection.push_back(tableMap[i + j * tableMapSize.x]);
//         }
//     }
// }
// void EditorState::paste_selection(sf::Vector2f const position)
// {
// }
// Apres
// void EditorState::update_selection(sf::Vector2f beginPos, sf::Vector2f
// endPos)
// {
//     beginPos = to_tile_position(beginPos - this->m_tileMapBegin) * 32.f +
//     this->m_tileMapBegin; endPos = to_tile_position(endPos -
//     this->m_tileMapBegin) * 32.f + this->m_tileMapBegin;
//     this->m_selectionRect.setPosition(beginPos);
//     this->m_selectionRect.setSize(endPos - beginPos);
// }

// Mouvement de la texture rect par rapport a la texture (Après, a voir si on va
// le garder) if(sheet.isPrint)
// {
//     sf::IntRect spriteSheetRect { this->m_tileset.get_rect() };
//     sf::Vector2i spriteSheetSize { (sf::Vector2i)this->m_tileset.get_size() };
//     if(this->m_keyboard["SpriteSheetUp"])
//     {
//         if(spriteSheetRect.top >= 32)
//         {
//             spriteSheetRect.top -= 32;
//         }
//     }
//     if(this->m_keyboard["SpriteSheetDown"])
//     {
//         if(spriteSheetRect.top + spriteSheetRect.height <= spriteSheetSize.y
//         - 32)
//         {
//             spriteSheetRect.top += 32;
//         }
//     }
//     if(this->m_keyboard["SpriteSheetLeft"])
//     {
//         if(spriteSheetRect.left >= 32)
//         {
//             spriteSheetRect.left -= 32;
//         }
//     }
//     if(this->m_keyboard["SpriteSheetRight"])
//     {
//         if(spriteSheetRect.left + spriteSheetRect.width <= spriteSheetSize.x
//         - 32)
//         {
//             spriteSheetRect.left += 32;
//         }
//     }
//     this->m_tileset.set_rect(spriteSheetRect);
// }
