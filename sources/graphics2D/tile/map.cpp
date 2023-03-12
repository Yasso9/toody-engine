#include "map.hpp"

#include <algorithm>  // for max
#include <cstdlib>    // for strtoul
#include <fstream>
#include <memory>   // for allocator_traits<>::value_...
#include <sstream>  // for operator<<, basic_ostream
#include <string>   // for char_traits, to_string

#include <IMGUI/imgui.h>                   // for Button, Text, InputText
#include <SFML/Graphics/Color.hpp>         // for Color, Color::Transparent
#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget
#include <SFML/Graphics/VertexArray.hpp>   // for VertexArray
#include <SFML/Window/Mouse.hpp>           // for Mouse, Mouse::Button, Mous...
#include <ext/alloc_traits.h>              // for __alloc_traits<>::value_type

#include "component/component.hpp"       // for Component::add_child
#include "graphics2D/constants.hpp"      // for TILE_PIXEL_SIZE_I, TILE_PI...
#include "graphics2D/sfml.hpp"           // for operator<<
#include "graphics2D/tile/position.hpp"  // for tile::Position, tile::Position...
#include "graphics2D/tile/set.hpp"       // for Tileset
#include "graphics2D/view.hpp"           // for View
#include "input/input.hpp"               // for get_mouse_position, is_pre...
#include "main/render.hpp"               // for Render
#include "maths/geometry/point.hpp"      // for PointF
#include "maths/geometry/point.tpp"      // for Point::Point<Type>, Point:...
#include "tools/array/vector.hpp"
#include "tools/assertion.hpp"  // for ASSERTION
#include "tools/path.hpp"
#include "tools/serialization.hpp"  // for Serializer, Unserializer
#include "tools/serialization.tpp"  // for Serializer::Serializer<Typ...
#include "tools/tools.tpp"          // for is_rectangle

/// @todo put a grid on the tilemap

/// @brief Stream to string
static std::string get_string ( std::ifstream & stream )
{
    std::streampos initialPosition { stream.tellg() };

    // ifstream to ostringstream
    std::ostringstream ostreamTemp {};
    ostreamTemp << stream.rdbuf();

    // Reset the pointer to initial position
    stream.seekg( initialPosition );

    // ostringstream to string
    return ostreamTemp.str();
}

/// @brief File to string
/// @param file File to read
/// @return Content of the file in a string, or nothing if the file doesn't
/// exist
static std::string get_file_content ( std::filesystem::path file )
{
    std::stringstream stream {};

    std::ifstream tilemapData { file };

    if ( tilemapData )
    {
        return get_string( tilemapData );
    }

    tilemapData.close();

    return "";
}

namespace tile
{
    Map::Map( View & view )
      : m_tileSelector {},
        m_table { m_tileSelector.get_tileset().get_texture() },
        m_cursor { Cursor::Outline },
        m_view { view },
        m_saveFile { path::get_folder( path::Data ) / "tilemap.txt" }
    {
        this->add_child( m_tileSelector );
        this->add_child( m_table );
        this->add_child( m_cursor );

        {  // Load table from file
            std::string content { get_file_content( m_saveFile ) };
            if ( content == "" )
            {
                content = "{ ( 1, 1 ) | [ (0, 0) ] }";
            }
            std::stringstream stream {};
            stream << content;
            stream >> m_table;
            std::cout << "Table " << m_table << std::endl;
        }

        this->setPosition( 0.f, 0.f );
    }

    void Map::update( UpdateContext context )
    {
        ImGui::SetNextWindowBgAlpha( 0.5f );
        if ( ImGui::Begin( "Tilemap Information" ) )
        {
            { /* UPDATE CURSOR */
                math::PointF mousePosition {
                    input::get_mouse_position_relative( context.window, m_view )
                        .to_point() };
                if ( ! this->get_tile_position( mousePosition ).has_value()
                     || ImGui::IsWindowHovered( ImGuiHoveredFlags_AnyWindow ) )
                {
                    // The mouse is outside the tilemap
                    m_cursor.hide();
                }
                else
                {
                    tile::Position tilePosition {
                        this->get_tile_position( mousePosition ).value() };
                    if ( input::is_pressed( context.window,
                                            sf::Mouse::Button::Left )
                         && m_tileSelector.get_tile_selected().has_value() )
                    {
                        // There's a left click and the mouse is inside the
                        // tilemap
                        this->change_tile(
                            tilePosition,
                            m_tileSelector.get_tile_selected().value() );
                    }

                    m_cursor.show_at_position(
                        tilePosition.pixel().to_float() );
                }
            }

            { /* UPDATE TILE SIZE BUTTON */
                static std::array< int, 2 > tileNumbers {
                    this->get_size().tile().to_int().x,
                    this->get_size().tile().to_int().y };

                ImGui::InputInt2( "Tilemap Size :", &tileNumbers[0],
                                  ImGuiInputTextFlags_CharsDecimal );

                if ( tileNumbers[0] != this->get_size().tile().to_int().x
                     || tileNumbers[1] != this->get_size().tile().to_int().y )
                {
                    std::stringstream stream {};
                    stream << "Current tilemap size : " << this->getPosition()
                           << "\n";
                    ImGui::Text( "%s", stream.str().c_str() );
                }

                if ( ImGui::Button( "Update Size" ) )
                {
                    m_table.set_size( tile::Size {
                        math::Vector2I { tileNumbers[0], tileNumbers[1] }
                            .to_u_int(),
                        tile::Size::Tile } );
                }
            }

            { /* UPDATE DEBUG */
                static bool showDebug { true };
                ImGui::Checkbox( "Show debug informations ?", &showDebug );
                if ( showDebug )
                {
                    std::ostringstream infoOutput {};

                    if ( m_tileSelector.get_tile_selected().has_value() )
                    {
                        infoOutput << "Tileset - Tile Selected : "
                                   << m_tileSelector.get_tile_selected()
                                          .value()
                                          .value()
                                   << "\n";
                    }
                    else
                    {
                        infoOutput << "Tileset - Tile Selected : None\n";
                    }

                    infoOutput << "Tilemap - Position : " << this->getPosition()
                               << "\n";
                    infoOutput
                        << "Tilemap - Size : " << this->get_size().pixel()
                        << "\n";
                    infoOutput << "Tilemap - Number of Tile : "
                               << this->get_size().tile() << "\n";

                    infoOutput << "View - Center : " << m_view.get_center()
                               << "\n";
                    infoOutput << "View - Size : " << m_view.get_size() << "\n";
                    infoOutput << "View - Position : " << m_view.get_position()
                               << "\n";
                    infoOutput
                        << "View - Zoom : " << m_view.get_zoom( context.window )
                        << "\n";

                    infoOutput << "Mouse Position - Absolute : "
                               << input::get_mouse_position( context.window )
                               << "\n";
                    infoOutput << "Mouse Position - Relativ to View : "
                               << input::get_mouse_position_relative(
                                      context.window, m_view )
                               << "\n";

                    ImGui::Text( "%s", infoOutput.str().c_str() );

                    /* TILEMAP DEBUG */
                    static std::ostringstream tileValueStream {};
                    if ( ImGui::BeginTable(
                             "Tile Table Debug",
                             static_cast< int >( this->get_size().tile().x ) ) )
                    {
                        for ( unsigned int line { 0u };
                              line < this->get_size().tile().y; ++line )
                        {
                            ImGui::TableNextRow();
                            for ( unsigned int column { 0u };
                                  column < this->get_size().tile().x; ++column )
                            {
                                ImGui::TableNextColumn();

                                std::stringstream buttonStream {};
                                buttonStream
                                    << math::Vector2 { column, line } << " | "
                                    << m_table.get_element( column, line )
                                           .get_value();

                                if ( ImGui::Button(
                                         buttonStream.str().c_str() ) )
                                {
                                    tileValueStream.str( "" );
                                    tileValueStream
                                        << "Tile"
                                        << "{ " << column << " - " << line
                                        << " } Informations :"
                                        << "\n"
                                        << m_table.get_element( column, line )
                                               .get_debug()
                                        << "\n";
                                }
                            }
                        }
                    }
                    ImGui::EndTable();
                    if ( ImGui::Button( "Clear Tile Information" ) )
                    {
                        tileValueStream.str( "" );
                    }

                    if ( ! tileValueStream.str().empty() )
                    {
                        ImGui::Text( "%s", tileValueStream.str().c_str() );
                    }
                }
            }

            if ( ImGui::Button( "Save Tilemap" ) )
            {
                std::cout << "Save" << std::endl;
                this->save();
            }
        }
        ImGui::End();
    }

    tile::Set const & Map::get_tileset() const
    {
        return this->m_tileSelector.get_tileset();
    };

    tile::Size Map::get_size() const
    {
        return m_table.get_size();
    }

    math::Vector2F Map::get_center_absolute() const
    {
        return this->get_center_relative()
               + math::Vector2F { this->getPosition() };
    }

    math::Vector2F Map::get_center_relative() const
    {
        return math::Vector2F { this->get_size().pixel().to_float() / 2.f };
    }

    std::optional< tile::Position > Map::get_tile_position(
        math::PointF point ) const
    {
        if ( ! this->contain( point ) )
        {
            return std::nullopt;
        }

        math::Vector2U pointRelative {
            point - math::Vector2F { this->getPosition() } };

        return tile::Position { pointRelative, this->get_size(),
                                tile::Position::Pixel };
    }

    bool Map::contain( math::PointF point ) const
    {
        if ( ! m_view.contain( point ) )
        {
            std::cerr << "The point isn't inside the view" << std::endl;
            return false;
        }

        return point.is_inside( math::PointF { this->getPosition() },
                                this->get_size().pixel().to_float() );
    }

    bool Map::is_comptatible( tile::Position position ) const
    {
        return this->get_size().tile().x == position.get_number_of_columns();
    }

    void Map::save() const
    {
        std::cout << "Saved" << std::endl;

        std::ofstream stream { m_saveFile, std::ios::out | std::ios::trunc };
        stream << m_table;
        stream.close();
    }

    void Map::change_tile( tile::Position position, tile::Position value )
    {
        /// @todo make a is_comptatible method
        if ( position >= this->get_size() )
        {
            std::cerr << "Tilemap position not comptatible with current tilemap"
                      << std::endl;
        }
        if ( value >= this->get_tileset().get_size() )
        {
            std::cerr << "Tileset position not comptatible with current tileset"
                      << std::endl;
        }

        m_table.get_element( position.tile().x, position.tile().y )
            .set_value( value );
    }
}  // namespace tile