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
                    "[ERROR]          {}\n", FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void Warning ( fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::orange ) | fmt::emphasis::bold,
                    "[WARNING]        {}\n", FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void Success ( fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::green ) | fmt::emphasis::bold,
                    "[SUCCESS]        {}\n", FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void Info ( fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::sky_blue ), "[INFO]           {}\n",
                    FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void Debug ( fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::gray ) | fmt::emphasis::italic,
                    "[DEBUG]          {}\n", FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void FileNotFound ( std::string const &           filePath,
                        fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::light_golden_rod_yellow ),
                    "[FILE NOT FOUND] '{}' : {}\n", filePath,
                    FORMAT_STRING( fmt, args ) );
    }

    template< typename... Args >
    void FileIssue ( std::string const &           filePath,
                     fmt::format_string< Args... > fmt, Args &&... args )
    {
        fmt::print( fmt::fg( fmt::color::light_golden_rod_yellow ),
                    "[FILE ISSUE]     '{}' : {}\n", filePath,
                    FORMAT_STRING( fmt, args ) );
    }
}  // namespace Trace
