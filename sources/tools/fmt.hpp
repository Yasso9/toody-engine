#pragma once

#include <fmt/format.h>

#define DEFINE_FORMATTER( Type, FormatStr, ... )                        \
    template<>                                                          \
    struct fmt::formatter< Type >                                       \
    {                                                                   \
        constexpr auto parse ( fmt::format_parse_context & ctx )        \
        {                                                               \
            return ctx.begin();                                         \
        }                                                               \
        template< typename FormatContext >                              \
        auto format ( Type const & value, FormatContext & ctx )         \
        {                                                               \
            return fmt::format_to( ctx.out(), FormatStr, __VA_ARGS__ ); \
        }                                                               \
    }

#define DEFINE_FORMATTER_TEMPLATE( Type, FormatStr, ... )               \
    template< typename T >                                              \
    struct fmt::formatter< Type< T > >                                  \
    {                                                                   \
        constexpr auto parse ( fmt::format_parse_context & ctx )        \
        {                                                               \
            return ctx.begin();                                         \
        }                                                               \
        template< typename FormatContext >                              \
        auto format ( Type< T > const & value, FormatContext & ctx )    \
        {                                                               \
            return fmt::format_to( ctx.out(), FormatStr, __VA_ARGS__ ); \
        }                                                               \
    }
