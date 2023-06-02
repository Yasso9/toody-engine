#pragma once

#include <string>  // for string

#include <fmt/format.h>  // for format

namespace Trace
{
    template< typename... Args >
    void Error ( fmt::format_string< Args... > fmt, Args &&... args );
    template< typename... Args >
    void Warning ( fmt::format_string< Args... > fmt, Args &&... args );
    template< typename... Args >
    void Success ( fmt::format_string< Args... > fmt, Args &&... args );
    template< typename... Args >
    void Info ( fmt::format_string< Args... > fmt, Args &&... args );
    template< typename... Args >
    void Debug ( fmt::format_string< Args... > fmt, Args &&... args );

    template< typename... Args >
    void FileNotFound ( std::string const &           filePath,
                        fmt::format_string< Args... > fmt = "",
                        Args &&... args );
    template< typename... Args >
    void FileIssue ( std::string const &           filePath,
                     fmt::format_string< Args... > fmt = "", Args &&... args );

    void GenerateTest ();
}  // namespace Trace

#include "traces_impl.hpp"
