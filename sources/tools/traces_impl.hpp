#pragma once

#include "traces.hpp"

#include <iostream>

#include <fmt/color.h>  // for color

#define FORMAT_STRING( fmt, args ) \
    fmt::format( fmt, std::forward< Args >( args )... )

namespace Trace
{
    template< typename... Args >
    void Error ( fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::orange_red ) | fmt::emphasis::bold,
                    "{:<15} {}\n", "[ERROR]", FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void Warning ( fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::orange ) | fmt::emphasis::bold,
                    "{:<15} {}\n", "[WARNING]", FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void Success ( fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::green ) | fmt::emphasis::bold,
                    "{:<15} {}\n", "[SUCCESS]", FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void Info ( fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::sky_blue ), "{:<15} {}\n", "[INFO]",
                    FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void Debug ( fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::gray ) | fmt::emphasis::italic,
                    "{:<15} {}\n", "[DEBUG]", FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void FileNotFound ( std::string const &           filePath,
                        fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::light_golden_rod_yellow ),
                    "{:<15} '{}' : {}\n", "[FILE NOT FOUND]", filePath,
                    FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void FileIssue ( std::string const &           filePath,
                     fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::light_golden_rod_yellow ),
                    "{:<15} '{}' : {}\n", "[FILE ISSUE]", filePath,
                    FORMAT_STRING( fmt, args ) );
    }
}  // namespace Trace
