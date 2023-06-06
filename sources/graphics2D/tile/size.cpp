#include "size.hpp"

namespace tile
{
    Size::Size() : Cell {} {}

    Size::Size( math::Vector2U size, Type type ) : Cell { size, type } {}

    Size::Size( unsigned int x, unsigned int y, Type type )
      : Cell { x, y, type }
    {}

    unsigned int Size::index() const
    {
        math::Vector2U tilePos { this->tile() };
        return tilePos.y * tilePos.x + tilePos.x;
    }

    void Size::index( unsigned int newValue )
    {
        this->set_tile_position(
            { newValue % this->tile().x,
              static_cast< unsigned int >( math::whole_part(
                  static_cast< float >( newValue )
                  / static_cast< float >( this->tile().x ) ) ) } );
    }

    Cell Size::to_cell() const
    {
        return *this;
    }
}  // namespace tile
