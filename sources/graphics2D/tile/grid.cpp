#include "grid.hpp"

#include "maths/geometry/point.hpp"  // for Point::is_inside
#include "tools/traces.hpp"          // for Trace::Warning

namespace tile
{
    math::Vector2F Grid::get_bound_pos() const
    {
        return this->get_position() + this->get_size().pixel().to_float();
    }

    math::Vector2F Grid::get_center_absolute() const
    {
        return this->get_center_relative() + this->get_position();
    }

    math::Vector2F Grid::get_center_relative() const
    {
        return math::Vector2F { this->get_size().pixel().to_float() / 2.f };
    }

    math::RectangleF Grid::get_rect() const
    {
        return { this->get_position(), this->get_size().pixel().to_float() };
    }

    std::optional< tile::CellPos > Grid::position(
        tile::Position const & cell ) const
    {
        if ( ! this->contains( cell ) )
        {
            return std::nullopt;
        }
        return tile::CellPos { cell, this->get_size() };
    }

    std::optional< tile::CellPos > Grid::position( unsigned int value ) const
    {
        if ( value >= this->get_size().nb_of_tile() )
        {
            return std::nullopt;
        }
        return tile::CellPos { value, this->get_size() };
    }

    std::optional< tile::CellPos > Grid::get_cell_pos(
        math::Vector2F const & absolutePos ) const
    {
        if ( ! this->contains_abs( absolutePos ) )
        {
            return std::nullopt;
        }

        math::Vector2U relativPos { absolutePos - this->get_position() };
        return this->position( { relativPos, tile::Type::Pixel } );
    }

    std::optional< math::Vector2F > Grid::get_cell_abs_pos(
        math::Vector2F const & absolutePos ) const
    {
        auto cellPos = this->get_cell_pos( absolutePos );
        if ( ! cellPos.has_value() )
        {
            return std::nullopt;
        }
        return cellPos.value().pixel_abs( this->get_position() ).to_float();
    }

    bool Grid::is_compatible( tile::CellPos const & position ) const
    {
        return this->get_size().tile().x == position.sizeX()
               && this->contains( position );
    }

    bool Grid::contains( tile::Position const & position ) const
    {
        return position.pixel().to_point().is_inside(
            { 0, 0 }, this->get_size().pixel() );
    }

    bool Grid::contains_abs( math::Vector2F const & absPos ) const
    {
        return this->get_rect().contains( absPos );
    }

}  // namespace tile
