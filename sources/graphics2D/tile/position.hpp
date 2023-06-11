#pragma once

#include "maths/vector2.hpp"  // for Vector2U

namespace tile
{
    enum class Type
    {
        Tile = 0,
        Pixel
    };

    // Base Tile class for CellPos and Size
    class Position
    {
      private:
        math::Vector2U m_tilePosition;
        math::Vector2U m_tileSizePixel;

      public:
        Position();
        Position( math::Vector2U position, Type type );
        Position( unsigned int x, unsigned int y, Type type );
        virtual ~Position() = default;

        math::Vector2U tile () const;
        math::Vector2U pixel () const;
        math::Vector2U pixel_abs ( math::Vector2F const & gridPos ) const;

        void tile ( math::Vector2U position );
        void pixel ( math::Vector2U position );
        void position ( math::Vector2U position, Type type );
        void position ( unsigned int x, unsigned int y, Type type );

      protected:
        virtual void set_tile_position ( math::Vector2U position );
    };
}  // namespace tile

DEFINE_FORMATTER( tile::Position, "{{ tile={} }}", value.tile() );
