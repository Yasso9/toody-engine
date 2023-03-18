#include "table.hpp"

#include "graphics2D/surface/quad_vertex.hpp"  // for QuadVertex
#include "graphics2D/tile/map.hpp"             // for TileMap
#include "graphics2D/tile/position.hpp"  // for tile::Position, tile::Position:...
#include "graphics2D/tile/set.hpp"       // for Tileset
#include "tools/array/vector.hpp"
#include "tools/assertion.hpp"

[[__maybe_unused__]] static unsigned int get_element_number (
    math::Vector2U position, math::Vector2U size );

namespace tile
{
    Table::Table( sf::Texture const & texture ) : m_texture { texture } {}

    void Table::update( UpdateContext context )
    {
        for ( Tile tile : m_array2D )
        {
            tile.update_all( context );
        }
    }

    void Table::render( RenderContext context ) const
    {
        context.set_texture( m_texture );

        for ( Tile tile : m_array2D )
        {
            tile.render_all( context );
        }
    }

    tile::Size Table::get_size() const
    {
        return tile::Size { this->Array2D< Tile >::get_size(),
                            tile::Size::Tile };
    }

    void Table::set_size( tile::Size size )
    {
        this->Array2D< Tile >::set_size( size.tile() );

        // If new tiles are added, we update there positions
        this->update_tiles_position();
    }

    void Table::update_tiles_position()
    {
        tile::Position i_position { math::Vector2U { 0u, 0u }, m_size.x,
                                    tile::Position::Tile };

        for ( Tile & tile : m_array2D )
        {
            tile.set_position( i_position );

            i_position.set_value( i_position.value() + 1 );
        }
    }

    std::istream & operator>> ( std::istream & stream, tile::Table & table )
    {
        stream >> static_cast< Array2D< tile::Tile > & >( table );

        table.update_tiles_position();

        return stream;
    }
}  // namespace tile

static unsigned int get_element_number ( math::Vector2U position,
                                         math::Vector2U size )
{
    return position.x + position.y * size.x;
}