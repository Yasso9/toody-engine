#include "editor_state.hpp"

#include <algorithm>  // for max, copy
#include <sstream>    // for operator<<, basic_ostream
#include <stddef.h>   // for NULL
#include <vector>     // for vector

#include <IMGUI/imgui.h>            // for Begin, End, MenuItem, Text
#include <SFML/Graphics/Color.hpp>  // for Color, Color::Black, Color::...
#include <SFML/System/Vector2.hpp>  // for Vector2f
#include <SFML/Window/Mouse.hpp>    // for Mouse, Mouse::Right

#include "graphics2D/component.tpp"      // for Component::add_child
#include "graphics2D/sfml.hpp"           // for operator<<
#include "graphics2D/static_entity.hpp"  // for StaticEntity2D
#include "graphics2D/view.hpp"           // for View
#include "input/input.hpp"               // for get_mouse_movement, get_mous...
#include "libraries/imgui.hpp"           // for P_Begin
#include "main/window.hpp"               // for Window
#include "maths/geometry/point.hpp"      // for PointF
#include "maths/geometry/polygon.hpp"    // for PolygonF
#include "maths/geometry/polygon.tpp"    // for Polygon::Polygon<Type>, Poly...
#include "maths/geometry/rectangle.hpp"  // for RectangleF
#include "maths/vector2.hpp"             // for Vector2F, Vector2, Vector2U
#include "maths/vector2.tpp"             // for operator<<, operator*, opera...
#include "tools/singleton.tpp"           // for Singleton::get_instance

EditorState::EditorState()
  : State { State::E_List::Editor },
    m_view { Window::get_instance().getDefaultView() },
    m_showWindow {
        {    "demo_window", false},
        {  "debug_options", false},
        {      "collision",  true},
        {"player_handling", false},
        {       "dialogue", false},
        {           "view", false}, },
    m_tilemap { m_view },
    m_imageMap {},
    m_collisionList {
        { StaticEntity2D { math::RectangleF { 100.f, 100.f, 50.f, 50.f } },
          StaticEntity2D { math::RectangleF { -300.f, 0.f, 200.f, 200.f } },
          StaticEntity2D { math::RectangleF { 0.f, 500.f, 100.f, 50.f } } } },
    m_greenEntity {
        math::RectangleF { 0.f, 0.f, 40.f, 40.f }, m_collisionList, m_view,
        input::ILKJ },
    m_player {},
    m_dialogue {}
{
    this->add_child( m_tilemap, m_view );
    this->add_child( m_collisionList, m_view );
    this->add_child( m_greenEntity, m_view );
    this->add_child( m_player, m_view );
    this->add_child( m_dialogue );
    this->add_child( m_imageMap );

    m_tilemap.setPosition( 0.f, 0.f );

    this->reset_view();

    m_greenEntity.setFillColor( sf::Color::Green );
    m_greenEntity.setOutlineColor( sf::Color::Black );
    m_greenEntity.setOutlineThickness( 2.f );

    m_dialogue.add_text(
        "Lorem Ipsum is simply dummy text of the printing and typesetting "
        "industry. Lorem Ipsum has been the industry's standard dummy text "
        "ever since the 1500s, when an unknown printer took a galley of type "
        "and scrambled it to make a type specimen book. It has survived not "
        "only five centuries, but also the leap into electronic typesetting, "
        "remaining essentially unchanged. It was popularised in the 1960s with "
        "the release of Letraset sheets containing Lorem Ipsum passages, and "
        "more recently with desktop publishing software like Aldus PageMaker "
        "including versions of Lorem Ipsum." );
}

void EditorState::update( float deltaTime )
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

    if ( m_showWindow.at( "dialogue" )
         && input::is_pressed( sf::Keyboard::Space ) )
    {
        m_dialogue.next();
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

    ///@todo use this function for all Imgui::P_Show
    ImGui::P_Show( "View Options", &m_showWindow.at( "view" ), [] () {
        ImGui::SliderFloat(
            "View Scroll Speed", &viewScrollSpeed, 0.f, 5.f, "%.2f" );
        ImGui::SliderFloat(
            "View Movement Speed", &viewMoveSpeedBase, 0.f, 50.f, "%.0f" );
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

    ImGui::P_Show( "View Options", &m_showWindow.at( "view" ), [&] () {
        std::stringstream output {};
        output << "View Center : " << m_view.get_center() << "\n";
        output << "View Position : " << m_view.get_position() << "\n";
        output << "View Size : " << m_view.get_size() << "\n";
        output << "Mouse Scroll : " << input::get_mouse_scroll() << "\n";
        output << "View Scroll Value : " << viewScrollValue << "\n";
        output << "Mouse Movement : " << input::get_mouse_movement() << "\n";
        output << "View Movement Speed : " << viewMoveSpeed << "\n";
        output << "View Movement Value : " << viewMoveValue << "\n";
        ImGui::Text( "%s", output.str().c_str() );
    } );

    m_view.zoom( viewScrollValue );
    m_view.move( viewMoveValue );
}

void EditorState::update_toolbar()
{
    if ( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu( "Options" ) )
        {
            // ImGui::MenuItem(
            //     "Show Editor Overlay Options", NULL,
            //     &m_showWindow.at( "editor_overlay" ) );
            // ImGui::EndMenu();
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
                         << math::Vector2F { ImGui::GetCursorPos() } << "\n";
        windowTextOutput << "CursorStartPos : "
                         << math::Vector2F { ImGui::GetCursorStartPos() }
                         << "\n";
        windowTextOutput << "CursorScreenPos : "
                         << math::Vector2F { ImGui::GetCursorScreenPos() }
                         << "\n";
        windowTextOutput << "ContentRegionAvail : "
                         << math::Vector2F { ImGui::GetContentRegionAvail() }
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

        for ( StaticEntity2D const & entity : m_collisionList.get_array() )
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
    ImGuiWindowFlags const window_flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing
        | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

    constexpr math::Vector2F const PADDINGS { 10.f, 10.f };
    math::Vector2F const           overlayPosition {
        math::Vector2F { ImGui::GetMainViewport()->WorkPos } + PADDINGS };

    ImGui::SetNextWindowPos( overlayPosition, ImGuiCond_Always );
    ImGui::SetNextWindowBgAlpha( 0.8f );
    if ( ImGui::Begin( "Editor Main", NULL, window_flags ) )
    {
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
                          << ImGui::IsWindowFocused(
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