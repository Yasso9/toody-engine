#pragma once

#include <filesystem>  // for path
#include <string>      // for string

// TODO delete this file. Use ressources.hpp instead
namespace path
{
    enum E_Folder
    {
        Data,
        Resources,
        Shaders,
        Tilesets,
        Character,
        Fonts,
        Samples,
    };

    // std::filesystem::path get_file ( E_File const & filePath );
    // std::string           get_file_str ( E_File const & filePath );

    std::filesystem::path get_folder ( E_Folder const & folderPath );
    std::string           get_folder_str ( E_Folder const & folderPath );
}  // namespace path
