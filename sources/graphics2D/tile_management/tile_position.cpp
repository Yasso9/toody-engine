#include "tile_position.hpp"

#include <cstdlib>  // for div, (anonymous), div_t
#include <memory>   // for allocator

#include "maths/vector2.tpp"          // for Vector2::operator=, floor, oper...
#include "tools/assertion.hpp"        // for ASSERTION
#include "tools/global_variable.hpp"  // for TILE_PIXEL_SIZE_VECTOR

namespace tile
{
    static math::Vector2U pixel_to_tile_position (
        math::Vector2U pixel_position );
    static math::Vector2U tile_to_pixel_position (
        math::Vector2U tile_position );

    /* ********************************************************************
     ***************************** POSITION *******************************
     ******************************************************************* */

    Position::Position( unsigned int value, unsigned int numberOfColumns )
      : m_position {}, m_numberOfColumns { numberOfColumns }
    {
        this->set_value( value );
    }

    Position::Position( unsigned int value, tile::Size mapSize )
      : Position { value, mapSize.tile().x }
    {}

    Position::Position(
        math::Vector2U position, unsigned int numberOfColumns, Type type )
      : m_position {}, m_numberOfColumns { numberOfColumns }
    {
        this->set_value( position, type );
    }

    Position::Position( math::Vector2U position, tile::Size mapSize, Type type )
      : Position { position, mapSize.tile().x, type }
    {}

    unsigned int Position::value() const
    {
        return m_position.x * m_numberOfColumns + m_position.y;
    }

    math::Vector2U Position::tile() const
    {
        return m_position;
    }

    math::Vector2U Position::pixel() const
    {
        return tile_to_pixel_position( this->tile() );
    }

    void Position::set_value( unsigned int value )
    {
        // Cast the tile value to a vector position
        m_position.x = value % m_numberOfColumns;
        m_position.y = static_cast< unsigned int >( math::whole_part(
            static_cast< float >( value )
            / static_cast< float >( m_numberOfColumns ) ) );

        /// @remark We don't know if value is too big for the grid
    }

    void Position::set_value( math::Vector2U position, Type type )
    {
        m_position = position;

        if ( type == Type::Pixel )
        {
            m_position = pixel_to_tile_position( position );
        }

        // Position too big for the current grid
        if ( m_position.x >= m_numberOfColumns )
        {
            throw std::out_of_range {
                "The position in the X axis can not be superior to the number "
                " of column " };
        }
    }

    /* ********************************************************************
     ***************************** POSITION *******************************
     ******************************************************************* */

    Size::Size( math::Vector2U size, Type type ) : m_size {}
    {
        this->set_value( size, type );
    }

    unsigned int Size::value() const
    {
        return m_size.x * m_size.x + m_size.y;
    }

    math::Vector2U Size::tile() const
    {
        return m_size;
    }

    math::Vector2U Size::pixel() const
    {
        return tile_to_pixel_position( this->tile() );
    }

    void Size::set_value( math::Vector2U position, Type type )
    {
        m_size = position;

        if ( type == Type::Pixel )
        {
            m_size = pixel_to_tile_position( position );
        }
    }

    /* ********************************************************************
     ***************************** STATICS ********************************
     ******************************************************************* */

    [[maybe_unused]] static math::Vector2U pixel_to_tile_position (
        math::Vector2U pixel_position )
    {
        return math::whole_part(
                   pixel_position.to_float() / TILE_PIXEL_SIZE_VECTOR )
            .to_u_int();
    }

    [[maybe_unused]] static math::Vector2U tile_to_pixel_position (
        math::Vector2U tile_position )
    {
        return tile_position * TILE_PIXEL_SIZE_VECTOR;
    }
}  // namespace tile