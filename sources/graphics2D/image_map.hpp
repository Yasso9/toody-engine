#pragma once

#include <vector> // for vector

#include <SFML/Graphics/Sprite.hpp> // for Sprite

#include "graphics2D/component.hpp" // for Component2D
#include "maths/geometry/point.hpp" // for PointU, Point

class Render;

struct S_Image
{
    sf::Sprite sprite {};
    math::PointU position { 0u, 0u };

    S_Image( sf::Sprite aSprite, math::PointU aPosition )
      : sprite( aSprite ), position( aPosition )
    {}
};

class ImageMap : public Component2D
{
    std::vector< S_Image > m_data;

  public:
    ImageMap();
    virtual ~ImageMap() = default;

    std::vector< S_Image > const & get_data() const;

  private:
    void render_before( Render & render ) const override;
};