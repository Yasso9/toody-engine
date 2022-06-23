#include "editor_state.hpp"

#include <memory>
#include <sstream>

#include "libraries/imgui.hpp"
#include "main/resources.hpp"
#include "tools/assertion.hpp"
#include "tools/string.hpp"

EditorState::EditorState()
  : State( State::E_List::Editor ),
    m_view(), // init in init_map
    m_tilemap( m_view ),
    m_player(),
    m_showDemoWindow( false ),
    m_showDebugOptions( false ),
    m_showEditorOverlay( true ),
    m_handlePlayer( false ),
    m_mousePosition( 0, 0 )
{
    this->init_map();
}

void EditorState::extra_events()
{
    this->m_mousePosition = sf::Mouse::getPosition( Window::get_instance() );

    this->m_tilemap.process_events();

    if ( ImGui::P_IsAnyWindowFocused() )
    {
        return;
    }

    /// @todo changer les events de la view pour pouvoir bouger la vue à partir de la souris (clique du milieu)
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )
    {
        this->m_view.move( 0.f, -5.f );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
    {
        this->m_view.move( 0.f, 5.f );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) )
    {
        this->m_view.move( -5.f, 0.f );
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
    {
        this->m_view.move( 5.f, 0.f );
    }

    if ( this->m_handlePlayer )
    {
        this->m_player.update_events();
    }
}

void EditorState::update()
{
    this->update_toolbar();

    this->update_overlay();

    if ( this->m_showDebugOptions )
    {
        this->update_debug_window();
    }
    if ( this->m_showDemoWindow )
    {
        ImGui::ShowDemoWindow( &this->m_showDemoWindow );
    }

    this->m_tilemap.update();

    if ( this->m_handlePlayer )
    {
        this->m_player.update( this->m_deltaTime );
        this->m_view.setCenter( this->m_player.getPosition() );
    }
}

void EditorState::render() const
{
    Window::get_instance().setView( this->m_view );

    Window::get_instance().sf_draw( this->m_tilemap );

    if ( this->m_handlePlayer )
    {
        Window::get_instance().sf_draw( this->m_player );
    }

    Window::get_instance().reset_view();
}

void EditorState::init_map()
{
    this->m_tilemap.setPosition( 0.f, 0.f );

    /// @todo create a methode for tilemap called .get_center who return the center position of the tilemap (relatively or asbolutely)
    // Set view position at center of the tilemap
    this->m_view.setCenter( math::Vector2F { this->m_tilemap.getPosition() }
                            + ( this->m_tilemap.get_size() / 2.f ) );
    this->m_view.setSize( Window::get_instance().get_size().to_float() );

    this->m_player.setPosition( this->m_view.get_center() );
}

void EditorState::mouse_scroll( float const & deltaScroll )
{
    if ( ImGui::P_IsAnyWindowFocused() )
    {
        return;
    }

    float const scaleFactor { 1.f - ( deltaScroll / 4.f ) };
    this->m_view.zoom( scaleFactor );

    // Check if the zoom doesn't go too far
    constexpr float MAXIMUM_ZOOM { 15.f };
    constexpr float MINIMUM_ZOOM { 0.7f };
    if ( this->m_view.get_zoom().get_max() > MAXIMUM_ZOOM )
    {
        this->m_view.set_zoom( MAXIMUM_ZOOM );
    }
    if ( this->m_view.get_zoom().get_min() < MINIMUM_ZOOM )
    {
        this->m_view.set_zoom( MINIMUM_ZOOM );
    }
}

void EditorState::keyboard_pressed( sf::Event event )
{
    if ( ImGui::P_IsAnyWindowFocused() )
    {
        return;
    }

    /// @todo put also that in the overlay
    if ( event.key.code == sf::Keyboard::C )
    {
        this->init_map();
    }
}

void EditorState::update_toolbar()
{
    if ( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu( "Options" ) )
        {
            ImGui::MenuItem( "Show Editor Overlay Options",
                             NULL,
                             &this->m_showEditorOverlay );
            ImGui::MenuItem( "Show ImGui Demo Window",
                             NULL,
                             &this->m_showDemoWindow );
            ImGui::MenuItem( "Show Debug Options",
                             NULL,
                             &this->m_showDebugOptions );
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void EditorState::update_debug_window()
{
    if ( ImGui::P_Begin( "Debug Options", &this->m_showDebugOptions ) )
    {
        std::string testBuffer { "testvalue" };
        ImGui::InputText( "Test", testBuffer.data(), 20 );

        std::stringstream windowTextOutput {};
        windowTextOutput << "MousePos : " << this->m_mousePosition << "\n";
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

        windowTextOutput << "\n";

        windowTextOutput << "IsAnyItemActive : " << std::boolalpha
                         << ImGui::IsAnyItemActive() << "\n";
        windowTextOutput << "IsAnyItemHovered : " << std::boolalpha
                         << ImGui::IsAnyItemHovered() << "\n";
        ImGui::Text( "%s", windowTextOutput.str().c_str() );
        ImGui::End();
    }
}

void EditorState::update_overlay()
{
    if ( this->m_showEditorOverlay )
    {
        ImGuiWindowFlags const window_flags =
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
            | ImGuiWindowFlags_NoMove;

        constexpr math::Vector2F const PADDINGS { 10.f, 10.f };
        math::Vector2F const overlayPosition {
            math::Vector2F { ImGui::GetMainViewport()->WorkPos } + PADDINGS
        };
        ImGui::SetNextWindowPos( overlayPosition, ImGuiCond_Always );
        ImGui::SetNextWindowBgAlpha( 0.5f );
        if ( ImGui::P_Begin( "Editor Main",
                             &this->m_showEditorOverlay,
                             window_flags ) )
        {
            ImGui::Checkbox( "Handle Player ?", &this->m_handlePlayer );

            static bool showDebug { false };
            ImGui::Checkbox( "Show Debug", &showDebug );
            if ( showDebug )
            {
                std::stringstream overlayOutput {};
                overlayOutput << "Window Position : " << overlayPosition
                              << "\n";
                ImGui::Text( "%s", overlayOutput.str().c_str() );
            }
        }
        ImGui::End();
    }
}