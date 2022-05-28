#include "editor_state.hpp"

#include <sstream>

#include "main/resources.hpp"
#include "tools/assertion.hpp"
#include "tools/imgui.hpp"
#include "tools/string.hpp"

EditorState::EditorState()
  : State( State::E_List::Editor ), m_tilemapEditor() /* ,
    m_tilemap( Resources::get_instance().get_texture(
        Resources::E_TextureKey::Tileset ) ),
    m_tileset( Resources::get_instance().get_texture(
        Resources::E_TextureKey::Tileset ) )*/
{
    this->init_map();
}

void EditorState::extra_events()
{
    // Movement de la mapView par raport a l'ecran,
    // deplacer la vue revient a deplacer la mapView
    // On ne met pas de else if pour pouvoir avoir un mouvement multiple
    // if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )
    // {
    //     this->m_tilemap.move( 0.f, 5.f );
    // }
    // if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
    // {
    //     this->m_tilemap.move( 0.f, -5.f );
    // }
    // if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) )
    // {
    //     this->m_tilemap.move( 5.f, 0.f );
    // }
    // if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
    // {
    //     this->m_tilemap.move( -5.f, 0.f );
    // }
}

void EditorState::update()
{
    ImGui::ShowDemoWindow();

    this->m_tilemapEditor.update();

    if ( ImGui::Begin( "Debug Options" ) )
    {
        std::stringstream windowTextOutput {};
        windowTextOutput << "MousePos : "
                         << sf::Vector2f { ImGui::GetMousePos() } << "\n";
        windowTextOutput << "CursorPos : "
                         << sf::Vector2f { ImGui::GetCursorPos() } << "\n";
        windowTextOutput << "CursorStartPos : "
                         << sf::Vector2f { ImGui::GetCursorStartPos() } << "\n";
        windowTextOutput << "CursorScreenPos : "
                         << sf::Vector2f { ImGui::GetCursorScreenPos() }
                         << "\n";
        windowTextOutput << "ContentRegionAvail : "
                         << sf::Vector2f { ImGui::GetContentRegionAvail() }
                         << "\n";

        windowTextOutput << "\n";

        windowTextOutput << "IsWindowFocused : " << std::boolalpha
                         << ImGui::IsWindowFocused() << "\n";
        windowTextOutput << "IsWindowHovered : " << ImGui::IsWindowHovered()
                         << "\n";
        ImGui::Text( "%s", windowTextOutput.str().c_str() );
        ImGui::End();
    }

    // this->m_tileset.update(
    //     static_cast< sf::Vector2f >( sf::Mouse::getPosition() ),
    //     sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) );

    // this->m_tilemap.update(
    //     static_cast< sf::Vector2f >( sf::Mouse::getPosition() ),
    //     static_cast< unsigned int >( this->m_tileset.get_selected_tile() ),
    //     sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) );
}

void EditorState::render() const
{
    // Window::get_instance().setView( this->m_view );

    // Window::get_instance().sf_draw( this->m_tilemap );
    // Window::get_instance().sf_draw( this->m_tileset );
}

void EditorState::init_map()
{
    // this->m_tilemap.setPosition(
    //     ( this->m_settings.get_window_size_f() - this->m_tilemap.get_size() )
    //     / 2.f );

    // // Init view
    // this->m_view.setCenter( this->m_settings.get_window_size_f() / 2.f );
    // this->m_view.setSize( this->m_settings.get_window_size_f() );

    // this->m_tileset.setPosition( this->m_settings.get_window_size_f() / 5.f );
    // // TYPO la taille ne doit pas être aberrante
    // this->m_tileset.set_size( this->m_settings.get_window_size_f() / 3.f );
}

void EditorState::mouse_scroll( float const & /* deltaScroll */ )
{
    // float const scaleValue { 1.f + ( deltaScroll / 4.f ) };
    // this->m_tilemap.scale( scaleValue, scaleValue );
}

void EditorState::keyboard_pressed( sf::Event /* event */ )
{
    // if ( event.key.code == sf::Keyboard::Num1 )
    // {
    //     this->m_tilemap.set_depth( 0 );
    // }
    // else if ( event.key.code == sf::Keyboard::Num2 )
    // {
    //     this->m_tilemap.set_depth( 1 );
    // }
    // else if ( event.key.code == sf::Keyboard::C )
    // {
    //     // TYPO mettre le centre de la vue et non pas la taille de la fenetre / 2
    //     this->m_tilemap.setPosition( this->m_settings.get_window_size_f()
    //                                  / 2.f );
    // }
    // else if ( event.key.code == sf::Keyboard::Space )
    // {
    //     this->m_tileset.switch_print();
    // }
    // else if ( event.key.code == sf::Keyboard::Enter )
    // {
    //     this->m_tilemap.save();
    // }
}