#include "editor_state.hpp"

EditorState::EditorState( std::map<TextureKey, sf::Texture> const & textures,
                          std::map<FontKey, sf::Font> const & fonts,
                          sf::Vector2u const & windowSize )
  : State( textures, fonts, windowSize, StateName::Editor )
{
    this->m_tile = 0;
    this->m_mode = Mode::Normal;

    this->init_map();
    this->init_widget();

    this->init_views();
    this->init_selection_rect();
}

void EditorState::init_map()
{
    this->m_tilemap.set_texture( this->m_textures.at( TextureKey::Tileset ) );
    this->m_tilemap.setPosition(
        ( static_cast<sf::Vector2f>( this->m_windowSize )
          - this->m_tilemap.get_size() )
        / 2.f );

    this->m_sheet.create( this->m_textures.at( TextureKey::Tileset ) );
    this->m_sheet.setPosition( sf::Vector2f( 150.f, 150.f ) );
}

void EditorState::init_widget()
{
    this->m_buttons = ButtonVector { this->m_fonts.at( FontKey::Arial ),
                                     { "Normal", "Selection" } };
    this->m_debugInfo.create( this->m_fonts.at( FontKey::Arial ), 4 );
}

void EditorState::init_views()
{
    this->m_screenView.reset(
        sf::FloatRect( 0.f,
                       0.f,
                       static_cast<float>( this->m_windowSize.x ),
                       static_cast<float>( this->m_windowSize.y ) ) );
    this->m_mapView.reset(
        sf::FloatRect( 0.f,
                       0.f,
                       static_cast<float>( this->m_windowSize.x ),
                       static_cast<float>( this->m_windowSize.y ) ) );
}

// Apres
void EditorState::init_selection_rect()
{
    this->m_selectionRect.setPosition( 0.f, 0.f );
    this->m_selectionRect.setSize( sf::Vector2f( 0.f, 0.f ) );
    this->m_selectionRect.setFillColor( sf::Color( 255, 230, 90, 128 ) );
}

void EditorState::handle_mouse_wheel_up()
{
    if ( this->m_mapView.getSize().x >= 50.f
         && this->m_mapView.getSize().y >= 20.f )
    {
        this->m_mapView.zoom( 0.8f );
    }
    if ( this->m_mapView.getSize().x
             < static_cast<float>( this->m_windowSize.x ) * 2.f
         && this->m_mapView.getSize().y
                < static_cast<float>( this->m_windowSize.y ) * 2.f )
    {
        this->m_mapView.zoom( 1.2f );
    }
}

void EditorState::handle_mouse_wheel_down()
{
    if ( this->m_mapView.getSize().x >= 50.f
         && this->m_mapView.getSize().y >= 20.f )
    {
        this->m_mapView.zoom( 0.8f );
    }
    if ( this->m_mapView.getSize().x
             < static_cast<float>( this->m_windowSize.x ) * 2.f
         && this->m_mapView.getSize().y
                < static_cast<float>( this->m_windowSize.y ) * 2.f )
    {
        this->m_mapView.zoom( 1.2f );
    }
}

void EditorState::handle_keyboard_press( std::string const & input )
{
    if ( input == "Profondeur1" )
    {
        this->m_tilemap.set_depth( 0 );
    }
    else if ( input == "Profondeur2" )
    {
        this->m_tilemap.set_depth( 1 );
    }
    else if ( input == "Center" )
    {
        this->init_views();
    }
    else if ( input == "PrintOrRemoveSpriteSheet" )
    {
        if ( this->m_sheet.isPrint == false )
            this->m_sheet.isPrint = true;
        else
            this->m_sheet.isPrint = false;
    }
    else if ( input == "Save" )
    {
        this->m_tilemap.save();
    }
    else if ( input == "MainMenu" )
    {
        this->m_stateName = StateName::MainMenu;
    }

    if ( this->m_mouseButton.at( "AcceptClick" ).second )
    {
        this->m_mode = static_cast<Mode>(
            this->m_buttons.update_press( this->m_mousePosition.get_press(),
                                          static_cast<int>( this->m_mode ) ) );
    }
}

void EditorState::handle_global_input()
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

    this->m_buttons.update_select( this->m_mousePosition.get_overall() );
}

void EditorState::update_normal_mode()
{
    this->m_sheet.update( this->m_mousePosition.get_overall(),
                          this->m_tile,
                          this->m_mouseButton.at( "Action" ).second );

    this->m_tilemap.update( to_view( this->m_mousePosition.get_overall(),
                                     this->m_screenView,
                                     this->m_mapView ),
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
    this->handle_global_input();

    // Mise a jour des actions en fonction du mode
    if ( this->m_mode == Mode::Normal )
    {
        this->update_normal_mode();
    }
    else if ( this->m_mode == Mode::Selection )
    {
        this->update_selection_mode();
    }
    // else if(this->m_mode == Mode::Colision)
    // {
    //     this->update_colision_mode();
    // }

    // Mise a jour des informations
    this->m_debugInfo.update(
        sf::Vector2f( 0.8f * static_cast<float>( this->m_windowSize.x ),
                      0.1f * static_cast<float>( this->m_windowSize.y ) ),

        { "Depth : " + std::to_string( this->m_tilemap.get_depth() ),
          "Mode : " + std::to_string( static_cast<int>( this->m_mode ) ),
          "Mouse : " + std::to_string( this->m_mousePosition.get_overall().x )
              + ", " + std::to_string( this->m_mousePosition.get_overall().y ),
          "Mouse Press : "
              + std::to_string( this->m_mousePosition.get_press().x ) + ", "
              + std::to_string( this->m_mousePosition.get_press().y ) } );
}

void EditorState::render( sf::RenderWindow & target )
{
    /* ########################## MAP VIEW ############################# */
    target.setView( this->m_mapView );

    target.draw( this->m_tilemap );

    /* ########################## WINDOW VIEW ############################# */
    target.setView( this->m_screenView );

    // La fonction regarde si le sprite sheet doit etre afficher
    this->m_sheet.render( target );

    this->m_buttons.render( target );
    this->m_debugInfo.render( target );
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
//     sf::IntRect spriteSheetRect { this->m_sheet.get_rect() };
//     sf::Vector2i spriteSheetSize { (sf::Vector2i)this->m_sheet.get_size() };
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
//     this->m_sheet.set_rect(spriteSheetRect);
// }
