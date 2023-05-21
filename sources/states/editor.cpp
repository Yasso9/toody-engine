#include "editor.hpp"

#include <algorithm>  // for max, copy
#include <sstream>    // for operator<<, basic_ostream
#include <stddef.h>   // for NULL
#include <vector>     // for vector

#include <SFML/Graphics/Color.hpp>  // for Color, Color::Black, Color::...
#include <SFML/System/Vector2.hpp>  // for Vector2f
#include <SFML/Window/Mouse.hpp>    // for Mouse, Mouse::Right
#include <imgui/imgui.h>            // for Begin, End, MenuItem, Text

#include "components/component.hpp"  // for Component::add_child
#include "contexts/game_context.hpp"
#include "game/resources.hpp"                   // for get_texture
#include "game/settings.hpp"                    // for Settings
#include "graphics2D/entity/static_entity.hpp"  // for StaticEntity2D
#include "graphics2D/sfml.hpp"                  // for operator<<
#include "graphics2D/view.hpp"                  // for View
#include "interface/window.hpp"                 // for Window
#include "libraries/imgui.hpp"                  // for P_Begin
#include "maths/geometry/point.hpp"             // for PointF
#include "maths/geometry/polygon.tpp"    // for Polygon::Polygon<Type>, Poly...
#include "maths/geometry/rectangle.hpp"  // for RectangleF
#include "maths/vector2.hpp"             // for Vector2F, Vector2, Vector2U
#include "maths/vector2.tpp"             // for operator<<, operator*, opera...
                                         // for get_mouse_movement, get_mous...
#include "tools/path.hpp"
#include "tools/singleton.tpp"  // for Singleton::get_instance

EditorState::EditorState()
  : State {},
    m_view {},
    m_viewSettings {},
    m_showWindow {
        { "demo_window", false }, { "debug_options", false },
        { "collision", false },   { "player_handling", false },
        { "dialogue", false },    { "view", false },
    },
    m_showDemoWindow { false },
    m_showOverlayWindow { false },
    m_tilemap { m_view },
    m_imageMap {},
    m_collisionList {},
    // m_greenEntity {
    //     math::RectangleF { 0.f, 0.f, 40.f, 40.f },
    //     { m_collisionList, m_view, input::ILKJ } },
    m_character { resources::get_texture( path::get_folder( path::Character )
                                          / "gold_sprite.png" ),
                  { m_collisionList, m_view, input::ARROW } }
{
    this->add_child( m_tilemap, m_view );
    this->add_child( m_collisionList, m_view );
    // this->add_child( m_greenEntity, m_view );
    this->add_child( m_character, m_view );
    this->add_child( m_imageMap );
    this->add_debug_window( m_view );

    m_tilemap.setPosition( 0.f, 0.f );

    this->reset_view( Settings::get_instance().get_window_size() );

    m_view.show_debug();

    // m_greenEntity.setFillColor( sf::Color::Green );
    // m_greenEntity.setOutlineColor( sf::Color::Black );
    // m_greenEntity.setOutlineThickness( 2.f );
}

void EditorState::update( UpdateContext & context )
{
    this->update_toolbar( context );
    this->update_view( context );
    this->update_overlay( context );

    if ( m_showWindow.at( "debug_options" ) )
    {  // UPDATE DEBUG
        ImGui::P_Show(
            "Debug Options", &m_showWindow.at( "debug_options" ), [&] () {
                std::stringstream windowTextOutput {};
                windowTextOutput << std::boolalpha;
                windowTextOutput
                    << "MousePos : " << context.inputs.get_mouse_position()
                    << "\n";
                windowTextOutput << "CursorPos : "
                                 << math::Vector2F { ImGui::GetCursorPos() }
                                 << "\n";
                windowTextOutput
                    << "CursorStartPos : "
                    << math::Vector2F { ImGui::GetCursorStartPos() } << "\n";
                windowTextOutput
                    << "CursorScreenPos : "
                    << math::Vector2F { ImGui::GetCursorScreenPos() } << "\n";
                windowTextOutput
                    << "ContentRegionAvail : "
                    << math::Vector2F { ImGui::GetContentRegionAvail() }
                    << "\n";

                windowTextOutput << "\n";

                windowTextOutput
                    << "IsWindowFocused : " << ImGui::IsWindowFocused() << "\n";
                windowTextOutput
                    << "IsWindowHovered : " << ImGui::IsWindowHovered() << "\n";

                windowTextOutput << "\n";

                windowTextOutput
                    << "IsAnyItemActive : " << ImGui::IsAnyItemActive() << "\n";
                windowTextOutput
                    << "IsAnyItemHovered : " << ImGui::IsAnyItemHovered()
                    << "\n";
                ImGui::Text( "%s", windowTextOutput.str().c_str() );
            } );
    }
}

void EditorState::update_toolbar( UpdateContext & context )
{
    bool quitEditor { false };
    bool resetView { false };

    if ( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu( "Options" ) )
        {
            ImGui::MenuItem( "Quit Editor", "Escape", &quitEditor );
            ImGui::MenuItem( "Reset View", "Ctrl + C", &resetView );
            ImGui::MenuItem( "Show Demo Window", "", &m_showDemoWindow );
            ImGui::MenuItem( "Show Overlay", "", &m_showOverlayWindow );
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if ( quitEditor )
    {
        context.transition_to( State::E_List::MainMenu );
    }
    if ( resetView )
    {
        this->reset_view( context.window.get_size().to_float() );
    }
    if ( m_showDemoWindow )
    {
        ImGui::ShowDemoWindow( &m_showDemoWindow );
    }
}

void EditorState::update_view( UpdateContext & context )
{
    // ImGui::P_Show( "View", &m_showViewWindow,
    //                [this] () { m_viewSettings.sliders(); } );

    float scrollValue { context.inputs.get_mouse_scroll().to_float().y
                        * m_viewSettings.zoomSpeed };
    m_view.zoom( scrollValue, context.window );

    math::Vector2F moveSpeed {
        math::Vector2F { m_viewSettings.moveSpeed, m_viewSettings.moveSpeed }
        / m_view.get_zoom( context.window ) };
    math::Vector2F const viewMoveValue {
        context.inputs.is_pressed( sf::Mouse::Right )
            ? context.inputs.get_mouse_movement().to_float() * moveSpeed
            : math::Vector2F { 0.f, 0.f } };
    m_view.m_sfView.move( viewMoveValue );
}

void EditorState::update_overlay( UpdateContext & context )
{
    if ( ! m_showOverlayWindow )
    {
        return;
    }

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
        | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

    constexpr math::Vector2F WINDOW_PADDINGS { 10.f, 10.f };
    math::Vector2F const     overlayPosition {
        math::Vector2F { ImGui::GetMainViewport()->WorkPos }
        + WINDOW_PADDINGS };

    ImGui::SetNextWindowPos( overlayPosition, ImGuiCond_Always );
    ImGui::SetNextWindowBgAlpha( 0.8f );
    if ( ImGui::Begin( "Editor Main", &m_showOverlayWindow, window_flags ) )
    {
        std::stringstream frameRateStream {};
        frameRateStream << "Frame Rate : "
                        << std::round( 1.f / context.deltaTime ) << "\n";
        ImGui::Text( "%s", frameRateStream.str().c_str() );

        static bool customizeCollision { false };
        ImGui::Checkbox( "Customize Collisions ?", &customizeCollision );
        m_collisionList.set_customisation( customizeCollision );

        static bool showDebug { false };
        ImGui::Checkbox( "Show Debug", &showDebug );
        if ( showDebug )
        {
            std::stringstream overlayOutput {};
            overlayOutput << "Window Position : " << overlayPosition << "\n";
            overlayOutput << "Is Any Window Focused ? " << std::boolalpha
                          << ImGui::IsWindowFocused(
                                 ImGuiFocusedFlags_AnyWindow )
                          << "\n";
            overlayOutput << "Is Any Window Hovered ? : "
                          << ImGui::IsWindowHovered(
                                 ImGuiHoveredFlags_AnyWindow )
                          << "\n";
            ImGui::Text( "%s", overlayOutput.str().c_str() );
        }

        if ( ImGui::TreeNode( "Show Tools" ) )
        {
            for ( auto & element : m_showWindow )
            {
                ImGui::Checkbox( element.first.c_str(), &element.second );
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void EditorState::reset_view( math::Vector2F const & windowSize )
{
    m_view.m_sfView.setCenter( m_tilemap.get_center_absolute() );
    m_view.m_sfView.setSize( windowSize );
    m_character.setPosition( m_view.get_center() );
}

// void EditorState::update_collision_window()
// {
// ImGui::P_Show( "Collisions", &m_showWindow.at( "collision" ), [&] () {
//     std::stringstream output {};
//     output << "Green Polygon : " << m_greenEntity.get_polygon().print()
//            << "\n";

// for ( StaticEntity2D const & entity : m_collisionList.get_entities() )
// {
//     output << "Intersection with " << entity.get_position() << " ? "
//            << std::boolalpha
//            << m_greenEntity.is_intersected_by( entity ) << "\n";
// }
// output << "Is collision detected : "
//        << m_greenEntity.is_collision_detected() << "\n";

// ImGui::Text( "%s", output.str().c_str() );
// } );
// }
