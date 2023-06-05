#include "editor.hpp"

#include <algorithm>                // for max, copy
#include <sstream>                  // for operator<<, basic_ostream
#include <stddef.h>                 // for NULL
#include <vector>                   // for vector

#include <SFML/Graphics/Color.hpp>  // for Color, Color::Black, Color::...
#include <SFML/System/Vector2.hpp>  // for Vector2f
#include <SFML/Window/Mouse.hpp>    // for Mouse, Mouse::Right
#include <imgui/imgui.h>            // for Begin, End, MenuItem, Text

#include "application/components/component.hpp"  // for Component::add_child
#include "application/configuration.hpp"         // for Settings
#include "application/contexts/game_context.hpp"
#include "application/interface/window.hpp"      // for Window
#include "application/resources.hpp"             // for get_texture
#include "graphics2D/entity/static_entity.hpp"   // for StaticEntity2D
#include "graphics2D/sfml.hpp"                   // for operator<<
#include "graphics2D/view.hpp"                   // for View
#include "imgui/imgui.hpp"                       // for P_Begin
#include "maths/geometry/point.hpp"              // for PointF
#include "maths/geometry/polygon.tpp"    // for Polygon::Polygon<Type>, Poly...
#include "maths/geometry/rectangle.hpp"  // for RectangleF
#include "maths/vector2.hpp"             // for Vector2F, Vector2, Vector2U
#include "maths/vector2.tpp"             // for operator<<, operator*, opera...
#include "tools/singleton.tpp"           // for Singleton::get_instance

EditorState::EditorState()
  : State {},
    m_view {},
    m_viewSettings {},
    m_showWindow { .demo = false, .overlay = false, .view = false },
    m_tilemap { m_view },
    m_imageMap {},
    m_collisionList {},
    m_character { resource::character::get( "gold_sprite.png" ),
                  { m_collisionList, m_view, input::ARROW } }
{
    this->add_child( m_tilemap, m_view );
    this->add_child( m_collisionList, m_view );
    this->add_child( m_character, m_view );
    this->add_child( m_imageMap );
    this->add_sub_window( m_view );
    this->add_sub_window( Config::get_instance() );

    m_tilemap.setPosition( 0.f, 0.f );
    this->reset_view( Config::get_instance().get_window_size() );
}

void EditorState::update( UpdateContext & context )
{
    this->update_toolbar( context );
    this->update_view( context );
    if ( m_showWindow.overlay )
    {
        this->update_overlay( context );
    }
}

void EditorState::update_toolbar( UpdateContext & context )
{
    bool quitEditor { false };
    bool resetView { false };
    bool generateTraces { false };

    if ( ImGui::BeginMainMenuBar() )
    {
        if ( ImGui::BeginMenu( "Options" ) )
        {
            ImGui::MenuItem( "Quit Editor", "Escape", &quitEditor );
            ImGui::MenuItem( "Reset View", "Ctrl + C", &resetView );
            ImGui::MenuItem( "Show Demo Window", "", &m_showWindow.demo );
            ImGui::MenuItem( "Show Overlay", "", &m_showWindow.overlay );
            ImGui::MenuItem( "Edit View Settings", "", &m_showWindow.view );

            ImGui::MenuItem( "Generate Traces", "", &generateTraces );
            ImGui::MenuItem( "Manage Settings", "",
                             &Config::get_instance().is_enabled() );

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if ( quitEditor )
    {
        context.transition_to( StateList::MainMenu );
    }
    if ( resetView )
    {
        this->reset_view( context.window.get_size().to_float() );
    }
    if ( m_showWindow.demo )
    {
        ImGui::ShowDemoWindow( &m_showWindow.demo );
    }
    if ( generateTraces )
    {
        Trace::GenerateTest();
    }
}

void EditorState::update_view( UpdateContext & context )
{
    if ( ImGui::BeginWindow( "View", m_showWindow.view ) )
    {
        m_viewSettings.sliders();
    }
    ImGui::End();

    // View Scroll
    float scrollY = context.inputs.get_mouse_scroll().to_float().y;
    if ( scrollY != 0.f )
    {
        m_view.zoom( scrollY * m_viewSettings.get_zoom_speed(),
                     context.window );
    }

    // View Movement
    math::Vector2I mouseMovement = context.inputs.get_mouse_movement();
    if ( mouseMovement != math::Vector2I::ZERO
         && context.inputs.is_pressed( sf::Mouse::Middle ) )
    {
        math::Vector2F moveSpeed { m_viewSettings.get_move_speed_vec()
                                   / m_view.get_zoom( context.window ) };
        m_view.m_sfView.move( mouseMovement.to_float() * moveSpeed );
    }
}

void EditorState::update_overlay( UpdateContext & context )
{
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration
                                   | ImGuiWindowFlags_AlwaysAutoResize
                                   | ImGuiWindowFlags_NoSavedSettings
                                   | ImGuiWindowFlags_NoFocusOnAppearing
                                   | ImGuiWindowFlags_NoNav
                                   | ImGuiWindowFlags_NoMove;
    constexpr math::Vector2F WINDOW_PADDINGS { 10.f, 10.f };
    math::Vector2F const     overlayPosition {
        math::Vector2F { ImGui::GetMainViewport()->WorkPos }
        + WINDOW_PADDINGS };

    ImGui::SetNextWindowPos( overlayPosition, ImGuiCond_Always );
    ImGui::SetNextWindowBgAlpha( 0.8f );
    if ( ImGui::BeginWindow( "Editor Main", m_showWindow.overlay,
                             windowFlags ) )
    {
        ImGui::TextFmt( "Frame Rate : {}",
                        std::round( 1.f / context.deltaTime ) );
        ImGui::TextFmt( "Window Position : {}", overlayPosition );
        ImGui::TextFmt( "Is Any Window Focused ? {}",
                        ImGui::IsWindowFocused( ImGuiFocusedFlags_AnyWindow ) );
        ImGui::TextFmt( "Is Any Window Hovered ? : {}",
                        ImGui::IsWindowHovered( ImGuiHoveredFlags_AnyWindow ) );
    }
    ImGui::End();
}

void EditorState::reset_view( math::Vector2F const & windowSize )
{
    m_view.m_sfView.setCenter( m_tilemap.get_center_absolute() );
    m_view.m_sfView.setSize( windowSize );
    m_character.setPosition( m_view.get_center() );
}
