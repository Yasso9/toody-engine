#include "editor_state.hpp"

#include <memory>
#include <sstream>

#include "libraries/imgui.hpp"
#include "main/resources.hpp"
#include "tools/assertion.hpp"
#include "tools/path.hpp"
#include "tools/string.hpp"

EditorState::EditorState()
  : State( State::E_List::Editor ),
    m_showWindow(),
    m_view(), // init in init_map
    m_tilemap( m_view ),
    // m_imageMap(),
    m_player(),
    m_collisionMap(),
    m_greenEntity( math::RectangleF { 0.f, 0.f, 40.f, 40.f }, m_collisionMap,
                   keyboard_move::ILKJ ),
    m_mousePosition( 0, 0 )
{
    this->init_map();

    m_showWindow = {
        {E_WindowKey::DemoWindow,      false},
        { E_WindowKey::DebugOptions,   false},
        { E_WindowKey::EditorOverlay,  false},
        { E_WindowKey::Collision,      true },
        { E_WindowKey::PlayerHandling, false},
    };

    m_greenEntity.setPosition( 0.f, 0.f );
    m_greenEntity.setFillColor( sf::Color::Green );
    m_greenEntity.setOutlineColor( sf::Color::Black );
    m_greenEntity.setOutlineThickness( 2.f );

    m_collisionMap.push_back( StaticEntity2D {
        math::RectangleF {100.f, 100.f, 50.f, 50.f}
    } );
    m_collisionMap.push_back( StaticEntity2D {
        math::RectangleF {-300.f, 0.f, 200.f, 200.f}
    } );
    m_collisionMap.push_back( StaticEntity2D {
        math::RectangleF {0.f, 500.f, 100.f, 50.f}
    } );
}

void EditorState::mouse_moved( sf::Event event )
{
    math::Vector2I mouseMovement { event.mouseMove.x, event.mouseMove.y };
    ( void )mouseMovement;
}

void EditorState::extra_events()
{
    m_tilemap.process_events();

    if ( ImGui::P_IsAnyWindowFocused() )
    {
        return;
    }

    m_greenEntity.update( m_deltaTime );

    constexpr float moveSpeedBaseValue { 10.f };
    math::Vector2F const moveSpeed {
        math::Vector2F {moveSpeedBaseValue, moveSpeedBaseValue}
        / m_view.get_zoom()
    };

    /// @todo changer les events de la view pour pouvoir bouger la vue à partir de la souris (clique du milieu)
    math::Vector2F moveDirection { 0.f, 0.f };
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Z ) )
    {
        moveDirection += math::Vector2F { 0.f, -1.f };
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::S ) )
    {
        moveDirection += math::Vector2F { 0.f, 1.f };
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Q ) )
    {
        moveDirection += math::Vector2F { -1.f, 0.f };
    }
    if ( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) )
    {
        moveDirection += math::Vector2F { 1.f, 0.f };
    }
    m_view.move( moveDirection.normalize() * moveSpeed );

    if ( m_showWindow[E_WindowKey::PlayerHandling] )
    {
        m_player.update_events();
    }
}

void EditorState::update()
{
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

    m_tilemap.update();

    if ( m_showWindow[E_WindowKey::PlayerHandling] )
    {
        m_player.update( m_deltaTime );
        m_view.setCenter( m_player.getPosition() );
    }

    this->update_overlay();
}

void EditorState::render() const
{
    Window::get_instance().setView( m_view );

    Window::get_instance().sf_draw( m_tilemap );

    for ( StaticEntity2D const & entity : m_collisionMap )
    {
        Window::get_instance().sf_draw( entity );
    }
    Window::get_instance().sf_draw( m_greenEntity );

    if ( m_showWindow.at( E_WindowKey::PlayerHandling ) )
    {
        Window::get_instance().sf_draw( m_player );
    }

    Window::get_instance().reset_view();
}

void EditorState::init_map()
{
    m_tilemap.setPosition( 0.f, 0.f );

    /// @todo create a methode for tilemap called .get_center who return the center position of the tilemap (relatively or asbolutely)
    // Set view position at center of the tilemap
    m_view.setCenter( math::Vector2F { m_tilemap.getPosition() }
                      + ( m_tilemap.get_size() / 2.f ) );
    m_view.setSize( Window::get_instance().get_size().to_float() );

    m_player.setPosition( m_view.get_center() );
}

void EditorState::mouse_scroll( float const & deltaScroll )
{
    if ( ImGui::P_IsAnyWindowHovered() )
    {
        return;
    }

    float const scaleFactor { 1.f - ( deltaScroll / 4.f ) };
    m_view.zoom( scaleFactor );

    // Check if the zoom doesn't go too far
    constexpr float MAXIMUM_ZOOM { 15.f };
    constexpr float MINIMUM_ZOOM { 0.7f };
    if ( m_view.get_zoom().get_max() > MAXIMUM_ZOOM )
    {
        m_view.set_zoom( MAXIMUM_ZOOM );
    }
    if ( m_view.get_zoom().get_min() < MINIMUM_ZOOM )
    {
        m_view.set_zoom( MINIMUM_ZOOM );
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
    if ( ImGui::P_Begin( "Debug Options",
                         &m_showWindow.at( E_WindowKey::DebugOptions ) ) )
    {
        std::string testBuffer { "testvalue" };
        ImGui::InputText( "Test", testBuffer.data(), 20 );

        std::stringstream windowTextOutput {};
        windowTextOutput << "MousePos : " << m_mousePosition << "\n";
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
    if ( ImGui::P_Begin( "Collisions",
                         &m_showWindow.at( E_WindowKey::Collision ) ) )
    {
        std::stringstream output {};
        output << "MousePos : " << m_mousePosition << "\n";
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
        if ( ImGui::P_Begin( "Editor Main",
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
                overlayOutput << "Is Any Window Focused ? " << std::boolalpha
                              << ImGui::P_IsAnyWindowFocused() << "\n";
                overlayOutput << "Is Any Window Hovered ? : "
                              << ImGui::P_IsAnyWindowHovered() << "\n";
                ImGui::Text( "%s", overlayOutput.str().c_str() );
            }
        }
        ImGui::End();
    }
}