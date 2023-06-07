#include "grid.hpp"

#include "maths/geometry/point.hpp"  // for Point::is_inside

namespace tile
{
    math::Vector2F Grid::get_bound_pos() const
    {
        return this->get_position() + this->get_size().pixel().to_float();
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

    bool Grid::is_compatible( tile::CellPos const & position ) const
    {
        return this->get_size().tile().x == position.sizeX()
               && this->contains( position );
    }

    bool Grid::contains( tile::Position const & position ) const
    {
        return position.pixel().to_point().is_inside(
            this->get_position().to_uint(), this->get_size().pixel() );
    }
}  // namespace tile
