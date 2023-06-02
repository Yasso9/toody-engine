#include "position.hpp"

#include <cstdlib>  // for div, (anonymous), div_t
#include <memory>   // for allocator
#include <sstream>

#include "graphics2D/tile/tools.hpp"
#include "maths/geometry/point.hpp"
#include "maths/vector2.tpp"    // for Vector2::operator=, floor, oper...
#include "tools/assertion.hpp"  // for ASSERTION

namespace tile
{
    Position::Position( unsigned int value, unsigned int numberOfColumns )
      : m_position {}, m_numberOfColumns { numberOfColumns }
    {
        this->set_value( value );
    }

    Position::Position( unsigned int value, tile::Size mapSize )
      : Position { value, mapSize.tile().x }
    {}

    Position::Position( math::Vector2U position, unsigned int numberOfColumns,
                        Type type )
      : m_position {}, m_numberOfColumns { numberOfColumns }
    {
        this->set_value( position, type );
    }

    Position::Position( math::Vector2U position, tile::Size mapSize, Type type )
      : Position { position, mapSize.tile().x, type }
    {}

    unsigned int Position::value() const
    {
        return m_position.y * m_numberOfColumns + m_position.x;
    }

    math::Vector2U Position::tile() const
    {
        return m_position;
    }

    math::Vector2U Position::pixel() const
    {
        return tile_to_pixel_position( this->tile() );
    }

    unsigned int Position::get_number_of_columns() const
    {
        return m_numberOfColumns;
    }

    void Position::set_value( unsigned int value )
    {
        // Cast the tile value to a vector position
        m_position.x = value % m_numberOfColumns;
        m_position.y = static_cast< unsigned int >(
            math::whole_part( static_cast< float >( value )
                              / static_cast< float >( m_numberOfColumns ) ) );

        /// @remark We don't know if value is too big for the grid
    }

    void Position::set_value( math::Vector2U position, Type type )
    {
        m_position = position;

        if ( type == Type::Pixel )
        {
            m_position = pixel_to_tile_position( m_position );
        }

        // Position too big for the current grid
        if ( m_position.x >= m_numberOfColumns )
        {
            std::cerr << "The position in the X axis can not be superior to "
                         "the number of column"
                      << std::endl;
        }
    }

    bool Position::is_inside( tile::Size size ) const
    {
        return this->tile().to_point().is_inside( { 0u, 0u }, size.tile() );
    }

    bool Position::operator<( tile::Size size ) const
    {
        return this->tile() < size.tile();
    }

    bool Position::operator>= ( tile::Size size ) const
    {
        return ! ( *this < size );
    }

    bool Position::operator<( tile::Position position ) const
    {
        return this->tile() < position.tile();
    }

    bool Position::operator>= ( tile::Position position ) const
    {
        return ! ( *this < position );
    }

    bool Position::operator== ( tile::Position position ) const
    {
        return this->m_position == position.m_position
               && this->m_numberOfColumns == position.m_numberOfColumns;
    }

    std::string Position::debug_string( std::string name ) const
    {
        if ( name != "" )
        {
            name += " ";
        }

        std::ostringstream stream {};
        stream << name << "pixel position : " << this->pixel() << "\n";
        stream << name << "tile position : " << this->tile() << "\n";
        stream << name << "tile value : " << this->value() << "\n";
        return stream.str().c_str();
    }
}  // namespace tile
