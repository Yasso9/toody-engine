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
#include "game/game.hpp"
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
#include "states/input.hpp"              // for get_mouse_movement, get_mous...
#include "tools/path.hpp"
#include "tools/singleton.tpp"  // for Singleton::get_instance

EditorState::EditorState( GameContext & gameContext )
  : State { gameContext },
    m_view {},
    m_showWindow {
        { "demo_window", false }, { "debug_options", false },
        { "collision", false },   { "player_handling", false },
        { "dialogue", false },    { "view", false },
    },
    m_tilemap { m_view },
    m_imageMap {},
    m_collisionList {},
    // m_greenEntity {
    //     math::RectangleF { 0.f, 0.f, 40.f, 40.f },
    //     { m_collisionList, m_view, input::ILKJ } },
    m_character { resources::get_texture( path::get_folder( path::Character )
                                          / "gold_sprite.png" ),
                  { m_collisionList, m_view, input::ARROW } },
    m_dialogue {}
{
    this->add_child( m_tilemap, m_view );
    this->add_child( m_collisionList, m_view );
    // this->add_child( m_greenEntity, m_view );
    this->add_child( m_character, m_view );
    this->add_child( m_dialogue );
    this->add_child( m_imageMap );

    m_tilemap.setPosition( 0.f, 0.f );

    this->reset_view( Settings::get_instance().get_window_size() );

    // m_greenEntity.setFillColor( sf::Color::Green );
    // m_greenEntity.setOutlineColor( sf::Color::Black );
    // m_greenEntity.setOutlineThickness( 2.f );

    m_dialogue.set_enabled( m_showWindow.at( "dialogue" ) );
}

void EditorState::update( UpdateContext context )
{
    {  // UPDATE VIEW
        static float viewScrollSpeed { 0.2f };
        static float viewMoveSpeedBase { 1.f };

        ImGui::P_Show( "View Options", &m_showWindow.at( "view" ), [] () {
            ImGui::SliderFloat( "View Scroll Speed", &viewScrollSpeed, 0.f, 5.f,
                                "%.2f" );
            ImGui::SliderFloat( "View Movement Speed", &viewMoveSpeedBase, 0.f,
                                50.f, "%.0f" );
        } );

        float const          viewScrollValue { input::get_mouse_scroll()
                                      * viewScrollSpeed };
        math::Vector2F const viewMoveSpeed {
            math::Vector2F { viewMoveSpeedBase, viewMoveSpeedBase }
            / m_view.get_zoom( context.window ) };
        math::Vector2F const viewMoveValue {
            input::is_pressed( context.window, sf::Mouse::Right )
                ? input::get_mouse_movement( context.window ) * viewMoveSpeed
                : math::Vector2F { 0.f, 0.f } };

        ImGui::P_Show( "View Options", &m_showWindow.at( "view" ), [&] () {
            std::stringstream output {};
            output << "View Center : " << m_view.get_center() << "\n";
            output << "View Position : " << m_view.get_position() << "\n";
            output << "View Size : " << m_view.get_size() << "\n";
            output << "Mouse Scroll : " << input::get_mouse_scroll() << "\n";
            output << "View Scroll Value : " << viewScrollValue << "\n";
            output << "Mouse Movement : "
                   << input::get_mouse_movement( context.window ) << "\n";
            output << "View Movement Speed : " << viewMoveSpeed << "\n";
            output << "View Movement Value : " << viewMoveValue << "\n";
            ImGui::Text( "%s", output.str().c_str() );
        } );

        m_view.zoom( viewScrollValue, context.window );
        m_view.move( viewMoveValue );
    }

    {  // UPDATE TOOLBAR
        bool        quitEditor { false };
        static bool resetView { false };
        static bool showDemo { false };

        if ( ImGui::BeginMainMenuBar() )
        {  // UPDATE TOOLBAR
            if ( ImGui::BeginMenu( "Options" ) )
            {
                ImGui::MenuItem( "Quit Editor", "Escape", &quitEditor );
                ImGui::MenuItem( "Reset View", "Ctrl + C", &resetView );
                ImGui::MenuItem( "Show Demo Window", "", &showDemo );
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if ( quitEditor )
        {
            m_gameContext.transition_to( State::E_List::MainMenu );
        }
        if ( resetView )
        {
            this->reset_view( context.window.get_size().to_float() );
            resetView = false;
        }
        if ( showDemo )
        {
            ImGui::ShowDemoWindow( &showDemo );
        }
    }

    {  // UPDATE OVERLAY
        ImGuiWindowFlags const window_flags =
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav
            | ImGuiWindowFlags_NoMove;

        constexpr math::Vector2F PADDINGS { 10.f, 10.f };
        math::Vector2F const     overlayPosition {
            math::Vector2F { ImGui::GetMainViewport()->WorkPos } + PADDINGS };

        ImGui::SetNextWindowPos( overlayPosition, ImGuiCond_Always );
        ImGui::SetNextWindowBgAlpha( 0.8f );
        ImGui::P_Show( "Editor Main", NULL, window_flags, [&] () {
            std::stringstream frameRateStream {};
            frameRateStream
                << "Frame Rate : " << std::round( 1.f / context.deltaTime )
                << "\n";
            ImGui::Text( "%s", frameRateStream.str().c_str() );

            static bool customizeCollision { false };
            ImGui::Checkbox( "Customize Collisions ?", &customizeCollision );
            m_collisionList.set_customisation( customizeCollision );

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
                    << ImGui::IsWindowHovered( ImGuiHoveredFlags_AnyWindow )
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

            m_dialogue.set_enabled( m_showWindow.at( "dialogue" ) );
        } );
    }

    if ( m_showWindow.at( "debug_options" ) )
    {  // UPDATE DEBUG
        ImGui::P_Show(
            "Debug Options", &m_showWindow.at( "debug_options" ), [&] () {
                std::stringstream windowTextOutput {};
                windowTextOutput << std::boolalpha;
                windowTextOutput << "MousePos : "
                                 << input::get_mouse_position( context.window )
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

void EditorState::reset_view( math::Vector2F const & windowSize )
{
    m_view.setCenter( m_tilemap.get_center_absolute() );
    m_view.setSize( windowSize );
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