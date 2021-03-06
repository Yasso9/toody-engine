#include "editor_state.hpp"

#include <memory>
#include <sstream>

#include "input/input.hpp"
#include "libraries/imgui.hpp"
#include "main/resources.hpp"
#include "tools/assertion.hpp"
#include "tools/path.hpp"
#include "tools/string.hpp"

EditorState::EditorState()
  : State( State::E_List::Editor ),
    m_showWindow {
        {"demo_window",      false},
        { "debug_options",   false},
        { "editor_overlay",  false},
        { "collision",       true },
        { "player_handling", false},
        { "view",            false},
},
    m_tilemap { m_view },
    m_imageMap {},
    m_collisionMap {
        StaticEntity2D { math::RectangleF { 100.f, 100.f, 50.f, 50.f } },
        StaticEntity2D { math::RectangleF { -300.f, 0.f, 200.f, 200.f } },
        StaticEntity2D { math::RectangleF { 0.f, 500.f, 100.f, 50.f } }
    },
    m_greenEntity { math::RectangleF { 0.f, 0.f, 40.f, 40.f },
                    m_collisionMap,
                    m_view,
                    input::ILKJ },
    m_player {}
{
    this->add_child( m_tilemap );
    this->add_childs( m_collisionMap );
    this->add_child( m_greenEntity );
    this->add_child( m_player );
    // this->add_child( m_imageMap );

    m_tilemap.setPosition( 0.f, 0.f );

    this->reset_view();

    m_greenEntity.setFillColor( sf::Color::Green );
    m_greenEntity.setOutlineColor( sf::Color::Black );
    m_greenEntity.setOutlineThickness( 2.f );
}

void EditorState::update_before( float deltaTime )
{
    this->update_toolbar();

    this->update_view( deltaTime );

    if ( m_showWindow.at( "debug_options" ) )
    {
        this->update_debug_window();
    }
    if ( m_showWindow.at( "collision" ) )
    {
        this->update_collision_window();
    }
    if ( m_showWindow.at( "demo_window" ) )
    {
        ImGui::ShowDemoWindow( &m_showWindow.at( "demo_window" ) );
    }

    this->update_overlay();
}

void EditorState::reset_view()
{
    m_view.setCenter( m_tilemap.get_center() );
    m_view.setSize( Window::get_instance().get_size().to_float() );
    m_player.setPosition( m_view.get_center() );
}

void EditorState::update_view( float /* deltaTime */ )
{
    static float viewScrollSpeed { 0.2f };
    static float viewMoveSpeedBase { 1.f };

    /// @todo use this function for all Imgui::P_Begin
    ImGui::P_Begin( "View Options",
                    &m_showWindow.at( "view" ),
                    []()
                    {
                        ImGui::SliderFloat( "View Scroll Speed",
                                            &viewScrollSpeed,
                                            0.f,
                                            5.f,
                                            "%.2f" );
                        ImGui::SliderFloat( "View Movement Speed",
                                            &viewMoveSpeedBase,
                                            0.f,
                                            50.f,
                                            "%.0f" );
                    } );

    float const viewScrollValue { input::get_mouse_scroll() * viewScrollSpeed };
    math::Vector2F const viewMoveSpeed {
        math::Vector2F {viewMoveSpeedBase, viewMoveSpeedBase}
        / m_view.get_zoom()
    };
    math::Vector2F const viewMoveValue {
        input::is_pressed( sf::Mouse::Right )
            ? input::get_mouse_movement() * viewMoveSpeed
            : math::Vector2F {0.f, 0.f}
    };

    if ( ImGui::Begin( "View Options" ) )
    {
        std::stringstream output {};
        output << "Mouse Scroll : " << input::get_mouse_scroll() << "\n";
        output << "View Scroll Value : " << viewScrollValue << "\n";
        output << "Mouse Movement : " << input::get_mouse_movement() << "\n";
        output << "View Movement Speed : " << viewMoveSpeed << "\n";
        output << "View Movement Value : " << viewMoveValue << "\n";
        ImGui::Text( "%s", output.str().c_str() );
    }
    ImGui::End();

    m_view.zoom( viewScrollValue );
    m_view.move( viewMoveValue );
}

void EditorState::update_toolbar()
{
    if ( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu( "Options" ) )
        {
            ImGui::MenuItem( "Show Editor Overlay Options",
                             NULL,
                             &m_showWindow.at( "editor_overlay" ) );
            ImGui::MenuItem( "Show ImGui Demo Window",
                             NULL,
                             &m_showWindow.at( "demo_window" ) );
            ImGui::MenuItem( "Show Debug Options",
                             NULL,
                             &m_showWindow.at( "debug_options" ) );
            ImGui::MenuItem( "Show View Options",
                             NULL,
                             &m_showWindow.at( "view" ) );
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void EditorState::update_debug_window()
{
    if ( ImGui::Begin( "Debug Options", &m_showWindow.at( "debug_options" ) ) )
    {
        std::string testBuffer { "testvalue" };
        ImGui::InputText( "Test", testBuffer.data(), 20 );

        std::stringstream windowTextOutput {};
        windowTextOutput << "MousePos : " << input::get_mouse_position()
                         << "\n";
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
    }
    ImGui::End();
}

void EditorState::update_collision_window()
{
    if ( ImGui::Begin( "Collisions", &m_showWindow.at( "collision" ) ) )
    {
        std::stringstream output {};
        output << "Green Polygon : " << m_greenEntity.get_polygon().print()
               << "\n";

        for ( StaticEntity2D const & entity : m_collisionMap )
        {
            output << "Intersection with " << entity.get_position() << " ? "
                   << std::boolalpha
                   << m_greenEntity.is_intersected_by( entity ) << "\n";
        }
        output << "Is collision detected : "
               << m_greenEntity.is_collision_detected() << "\n";

        ImGui::Text( "%s", output.str().c_str() );
    }
    ImGui::End();
}

void EditorState::update_overlay()
{
    if ( m_showWindow.at( "editor_overlay" ) )
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
        if ( ImGui::Begin( "Editor Main",
                           &m_showWindow.at( "editor_overlay" ),
                           window_flags ) )
        {
            ImGui::Checkbox( "Handle Player ?",
                             &m_showWindow.at( "player_handling" ) );

            static bool showDebug { false };
            ImGui::Checkbox( "Show Debug", &showDebug );
            if ( showDebug )
            {
                std::stringstream overlayOutput {};
                overlayOutput << "Window Position : " << overlayPosition
                              << "\n";
                overlayOutput
                    << "Is Any Window Focused ? " << std::boolalpha
                    << ImGui::IsWindowFocused( ImGuiFocusedFlags_AnyWindow )
                    << "\n";
                overlayOutput
                    << "Is Any Window Hovered ? : "
                    << ImGui::IsWindowFocused( ImGuiHoveredFlags_AnyWindow )
                    << "\n";
                ImGui::Text( "%s", overlayOutput.str().c_str() );
            }
        }
        ImGui::End();
    }
}