#pragma once

#include <project/output-info/information.hpp>
#include <project/utility/utility.hpp>

class TileMap : public sf::Drawable, public sf::Transformable
{
  public:
    TileMap( sf::Texture const & texture );

    /// @brief size of the tilemap in pixel
    sf::Vector2f get_size() const;

    virtual ~TileMap() = default;

  protected:
    sf::Texture const m_texture {};
    std::vector<std::vector<std::vector<sf::VertexArray>>> m_vertices {};

    /** @brief tri-dimensionnal vector containing the sprite number
     * of each tile of the tilemap */
    std::vector<std::vector<std::vector<unsigned int>>> m_table {};

    void synchronize_vertices();
    void get_depth( unsigned int const & depth ) const noexcept;

    virtual void draw( sf::RenderTarget & target,
                       sf::RenderStates states ) const override;
};

class EditorMap final : public TileMap
{
  public:
    EditorMap( sf::Texture const & texture );

    void set_actual_depth( unsigned int const & actualDepth );
    unsigned int get_actual_depth() const;

    void save() const;
    void resize();

    void update( sf::Vector2f const cursorPosition, unsigned int const tile,
                 bool const inputIsPress );

    ~EditorMap() = default;

  private:
    Cursor m_cursor {};

    /// @brief the depth that the user in the editor is actually working
    unsigned int m_actualDepth { 0u };

    void change_tile( sf::Vector2u const tilePosition,
                      unsigned int const tile );
    // void draw( sf::RenderTarget & target,
    //            sf::RenderStates states ) const override;
};