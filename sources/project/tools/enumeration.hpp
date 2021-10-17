#pragma once

// Use when needed to do operation on enum
template <typename Type>
class Enum
{
  public:
    Enum( Type const & enumValue );

    Enum( Enum const & ) = default;
    Enum & operator=( Enum const & ) = default;

    // Increment and decrement operator
    Enum & operator++();
    Enum operator++( int );
    Enum & operator--();
    Enum operator--( int );

    bool operator<( Type const & enumValue ) noexcept;
    bool operator>( Type const & enumValue ) noexcept;

    static Type get_min() noexcept;
    static Type get_max() noexcept;

    Type get_value() const noexcept;

    virtual ~Enum() {};

  protected:
    Type m_currentValue;
};

enum class Direction
{
    Up = 0,
    Down,
    Right,
    Left,
    EnumLast,
};

/// @brief directions that the character can have
// class Direction : public Enum
// {
//     Up = 0, Down, Right, Left, EnumLast,
// };

enum class PlayerState
{
    Normal = 0,
    Walking,
    Running,
    Poketech,
    Cycling,
    PokemonCenter,
    Watering,
    Surfing,
    UsingPokemon,
    Fishing,
    Saving,
    EnumLast,
};

enum class TextureKey
{
    Tileset = 0,
    Player,
    HomeWallpaper,
    EnumLast,
};

/// @brief All States that the game can have
enum class StateName
{
    MainMenu = 0,
    Game,
    Editor,
    Quit,
    EnumLast,
};

enum class FontKey
{
    Arial = 0,
    EnumLast,
};