#pragma once

#include <project/utility/utility.hpp>
#include <project/output-info/information.hpp>
#include <project/utility/read_file.hpp>

constexpr unsigned int g_totalDepth { 2 };

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap();

    void set_texture( sf::Texture const & texture );

    sf::Vector2f get_size() const;

    virtual ~TileMap() = default;
protected:
    std::shared_ptr<sf::Texture const> m_texture { nullptr };
    std::array<sf::VertexArray, g_totalDepth> m_vertices {};

    /// @brief vector containing the value of each tile of the tilemap
    std::array< std::vector<int>, g_totalDepth > m_table {};

    /// @brief number of tiles in the tilemap
    sf::Vector2u m_tileNumber {};
    /// @brief size of the tilemap in pixel
    sf::Vector2f m_size {};

    std::string const m_fileName {};

    /* Génère les valeurs des variables de la classe a partir d'un fichier */
    void load();
    /* Génère les valeurs en fonction de la texture dans les vertex
    de la variables "vertices". Doit être utilisé après load() */
    void reset();

    virtual void draw( sf::RenderTarget & target, sf::RenderStates states ) const;
};




class EditorMap final : public TileMap
{
public:
    EditorMap();

    void set_depth( unsigned int const & actualDepth );
    unsigned int get_depth() const;

    void save() const;
    void resize();

    void update( sf::Vector2f const cursorPosition, unsigned int const tile, bool const inputIsPress );

    ~EditorMap() = default;

private:
    TileCursor m_tileCursor {};

    bool m_isMouseOn {};

    /// @brief the depth that the user in the editor is actually working
    unsigned int m_actualDepth {};

    void change_tile( sf::Vector2u const tilePosition, unsigned int const tile );
    void draw( sf::RenderTarget & target, sf::RenderStates states ) const;
};