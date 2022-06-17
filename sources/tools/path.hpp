#pragma once

#include <filesystem>

#include "tools/assertion.hpp"

namespace path
{
    enum class E_File
    {
        Database = 0,
    };

    enum class E_Folder
    {
        Data = 0,
        Resources,
        Shaders,
    };

    std::filesystem::path get_file( E_File const & filePath );
    std::filesystem::path get_file_str( E_File const & filePath );

    std::filesystem::path get_folder( E_Folder const & folderPath );
    std::filesystem::path get_folder_str( E_Folder const & folderPath );
} // namespace path
