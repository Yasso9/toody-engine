#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#include "application/components/component.hpp"
#include "graphics2D/tile/position.hpp"
#include "graphics2D/tile/tile.hpp"
#include "maths/vector2.hpp"
#include "tools/array/array2D.hpp"
#include "tools/stream/stream.hpp"

namespace tile
{
    class Table : public Component,
                  public Array2D< Tile >
    {
        sf::Texture const & m_texture;

      public:
        Table( sf::Texture const & m_texture );
        ~Table() = default;

      private:
        void update ( UpdateContext & context ) override;
        void render ( RenderContext & context ) const override;

      public:
        tile::Size get_size () const;
        void       set_size ( tile::Size size );

        friend std::istream & operator>> ( std::istream & stream,
                                           Table &        table );  // override

      private:
        using Array2D< Tile >::size;
        using Array2D< Tile >::resize;

        /// @brief Update all the tiles position as they should be
        void update_tiles_position ();
    };
}  // namespace tile
