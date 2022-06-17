#pragma once

#include "maths/maths.hpp"
#include "tools/assertion.hpp"
#include "tools/global_variable.hpp"

struct S_TilePosition
{
    unsigned int value;
    unsigned int numberOfTileInX;

    enum Type
    {
        Tile = 0,
        Pixel
    };

    S_TilePosition( unsigned int aValue, unsigned int aNumberOfTileInX )
      : value( aValue ), numberOfTileInX( aNumberOfTileInX )
    {}
    S_TilePosition( math::Vector2U position, unsigned int aNumberOfTileInX,
                    Type type )
      : S_TilePosition { 0u, aNumberOfTileInX }
    {
        ASSERTION( type == Tile || type == Pixel, "Enum not handled" );

        math::Vector2U tilePosition { position };

        if ( type == Pixel )
        {
            tilePosition =
                math::floor( position.to_float() / TILE_PIXEL_SIZE_VECTOR )
                    .to_u_int();
        }

        this->value = tilePosition.x + ( tilePosition.y * numberOfTileInX );
    }

    unsigned int value() const { return this->value; }
    math::Vector2U tile() const
    {
        std::div_t divisionValue {
            std::div( this->value, static_cast< int >( this->numberOfTileInX ) )
        };
        return math::Vector2I { divisionValue.rem, divisionValue.quot }
            .to_u_int();
    }
    math::Vector2U pixel() const
    {
        return this->tile() * TILE_PIXEL_SIZE_VECTOR;
    }
};