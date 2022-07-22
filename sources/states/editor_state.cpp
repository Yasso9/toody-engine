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
        {E_WindowKey::DemoWindow,      false},
        { E_WindowKey::DebugOptions,   false},
        { E_WindowKey::EditorOverlay,  false},
        { E_WindowKey::Collision,      true },
        { E_WindowKey::PlayerHandling, false},
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
    this->add_child( m_imageMap );

    m_tilemap.setPosition( 0.f, 0.f );

    // Set view position at center of the tilemap
    m_view.setCenter( m_tilemap.get_center() );
    m_view.setSize( Window::get_instance().get_size().to_float() );

    m_player.setPosition( m_view.get_center() );

    m_greenEntity.setPosition( 0.f, 0.f );
    m_greenEntity.get_shape().setFillColor( sf::Color::Green );
    m_greenEntity.get_shape().setOutlineColor( sf::Color::Black );
    m_greenEntity.get_shape().setOutlineThickness( 2.f );
}

void EditorState::update_extra( float deltaTime )
{
    m_greenEntity.update( deltaTime );

    constexpr float moveSpeedBaseValue { 10.f };
    math::Vector2F const moveSpeed {
        math::Vector2F {moveSpeedBaseValue, moveSpeedBaseValue}
        / m_view.get_zoom()
    };

    if ( input::is_pressed( sf::Mouse::Middle ) )
    {
        m_view.move( input::get_mouse_movement().normalize() * moveSpeed );
    }

    m_view.zoom( input::get_mouse_scroll() / 4.f );

    this->update_toolbar();

    if ( m_showWindow[E_WindowKey::DebugOptions] )
    {
        this->update_debug_window();
    }
    if ( m_showWindow[E_WindowKey::Collision] )
    {
        this->update_collision_window();
    }
    if ( m_showWindow[E_WindowKey::DemoWindow] )
    {
        ImGui::ShowDemoWindow( &m_showWindow.at( E_WindowKey::DemoWindow ) );
    }

    this->update_overlay();
}

// void EditorState::render() const
// {
//     Window::get_instance().setView( m_view );

//     Window::get_instance().sf_draw( m_tilemap );

//     for ( StaticEntity2D const & entity : m_collisionMap )
//     {
//         Window::get_instance().sf_draw( entity );
//     }
//     Window::get_instance().sf_draw( m_greenEntity );

//     if ( m_showWindow.at( E_WindowKey::PlayerHandling ) )
//     {
//         Window::get_instance().sf_draw( m_player );
//     }

//     Window::get_instance().reset_view();
// }

void EditorState::update_toolbar()
{
    if ( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu( "Options" ) )
        {
            ImGui::MenuItem( "Show Editor Overlay Options",
                             NULL,
                             &m_showWindow.at( E_WindowKey::EditorOverlay ) );
            ImGui::MenuItem( "Show ImGui Demo Window",
                             NULL,
                             &m_showWindow.at( E_WindowKey::DemoWindow ) );
            ImGui::MenuItem( "Show Debug Options",
                             NULL,
                             &m_showWindow.at( E_WindowKey::DebugOptions ) );
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void EditorState::update_debug_window()
{
    if ( ImGui::Begin( "Debug Options",
                       &m_showWindow.at( E_WindowKey::DebugOptions ) ) )
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
    if ( ImGui::Begin( "Collisions",
                       &m_showWindow.at( E_WindowKey::Collision ) ) )
    {
        std::stringstream output {};
        output << "MousePos : " << input::get_mouse_position() << "\n";
        // ImGui::SliderScalar( "Green Speed",
        //                      ImGuiDataType_S8,
        //                      &s8_v,
        //                      &s8_min,
        //                      &s8_max,
        //                      "%d" );
        // output << "Green Speed : " << m_greenEntity.get_speed() << "\n";
        output << "Green Polygon : " << m_greenEntity.get_polygon().print()
               << "\n";

        for ( StaticEntity2D const & entity : m_collisionMap )
        {
            output << "Intersection with " << entity.getPosition() << " ? "
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
    if ( m_showWindow[E_WindowKey::EditorOverlay] )
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
                           &m_showWindow.at( E_WindowKey::EditorOverlay ),
                           window_flags ) )
        {
            ImGui::Checkbox( "Handle Player ?",
                             &m_showWindow.at( E_WindowKey::PlayerHandling ) );

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