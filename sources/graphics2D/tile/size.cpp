#include "size.hpp"

namespace tile
{
    Size::Size() : Position {} {}

    Size::Size( math::Vector2U size, Type type ) : Position { size, type } {}

    Size::Size( unsigned int x, unsigned int y, Type type )
      : Position { x, y, type }
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

    Position Size::to_cell() const
    {
        return *this;
    }

    unsigned int Size::nb_of_tile() const
    {
        return this->tile().x * this->tile().y;
    }
}  // namespace tile
