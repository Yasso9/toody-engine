#include "tileset.hpp"

#include <project/tools/geometry.hpp>
#include <project/tools/global_variable.hpp>

Tileset::Tileset( sf::Texture const & texture ) : m_image( texture ), m_cursor()
{
    this->setPosition( 0.f, 0.f );
}

sf::Vector2f Tileset::get_size() const noexcept
{
    sf::Vector2f const textureSize {
        static_cast<float>( this->m_image.getTextureRect().width ),
        static_cast<float>( this->m_image.getTextureRect().height )
    };

    return textureSize * this->getScale();
}

void Tileset::set_size( sf::Vector2f const & size ) noexcept
{
    this->setScale( size / this->get_size() );
}

void Tileset::set_size( float const & sizeX, float const & sizeY ) noexcept
{
    this->set_size( sf::Vector2f { sizeX, sizeY } );
}

int Tileset::get_selected_tile() const noexcept
{
    return this->m_tileSelected;
}

void Tileset::switch_print()
{
    this->m_isPrint = ! this->m_isPrint;
}

void Tileset::update_cursor( sf::Vector2f const & position )
{
    sf::Vector2f const cursorPosition { ( position - this->getPosition() )
                                        - ( position % ::g_tileSize_u )
                                        + this->getPosition() };

    this->m_cursor.update( cursorPosition,
                           sfpp::make_vector( ::g_tileSize_u ) );
}

bool Tileset::include( sf::Vector2f const & position ) const
{
    return tools::is_in_part( position, this->getPosition(), this->get_size() );
}

sf::Vector2u Tileset::get_tile_position( sf::Vector2f const & position ) const
{
    // Position compared to the tilemap Orign
    sf::Vector2f const positionComparedToTilemap { position
                                                   - this->getPosition() };
    sf::Vector2u const tilePosition { positionComparedToTilemap
                                      % ::g_tileSize_u };
    return tilePosition;
}

void Tileset::update( sf::Vector2f const & mousePosition,
                      bool const & buttonIsPressed )
{
    if ( ! this->m_isPrint || ! this->include( mousePosition ) )
    {
        return;
    }

    this->update_cursor( mousePosition );

    if ( buttonIsPressed )
    {
        // std::cout << this->get_tile_position( mousePosition ) << std::endl;

        // tile = tilesetPosition.x
        //        + tilesetPosition.y
        //              * static_cast<unsigned int>(
        //                  this->m_image.getTextureRect().width )
        //              / static_cast<unsigned int>( ::g_tileSize_f );
    }
}

void Tileset::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    // We draw only if it's must be printed
    if ( this->m_isPrint )
    {
        target.draw( this->m_image, states );
        target.draw( this->m_cursor );
    }
}