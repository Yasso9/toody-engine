#include "path.hpp"

namespace path
{
    std::filesystem::path get_file( E_File const & filePath )
    {
        std::filesystem::path fileRelativPath {};

        switch ( filePath )
        {
        case E_File::Database :
            fileRelativPath =
                get_folder( E_Folder::Data ) / "databases/game.db";
            break;
        default :
            ASSERTION( false, "Enum not handled in switch operation" );
            break;
        }

        if ( ! std::filesystem::is_regular_file( fileRelativPath ) )
        {
            throw std::runtime_error { "Error with file : '"
                                       + fileRelativPath.string() + "'" };
        }

        return fileRelativPath;
    }

    std::filesystem::path get_file_str( E_File const & filePath )
    {
        return get_file( filePath ).string();
    }

    std::filesystem::path get_folder( E_Folder const & folderPath )
    {
        std::filesystem::path folderRelativPath {};

        switch ( folderPath )
        {
        case E_Folder::Data :
            folderRelativPath = std::filesystem::path { "data" };
            break;
        case E_Folder::Resources :
            folderRelativPath = get_folder( E_Folder::Data ) / "resources";
            break;
        case E_Folder::Shaders :
            folderRelativPath = get_folder( E_Folder::Resources ) / "shaders";
            break;
        default :
            ASSERTION( false, "Enum not handled in switch operation" );
            break;
        }

        if ( ! std::filesystem::is_directory( folderRelativPath ) )
        {
            throw std::runtime_error { "Error with folder : '"
                                       + folderRelativPath.string() + "'" };
        }

        return folderRelativPath;
    }

    std::filesystem::path get_folder_str( E_Folder const & folderPath )
    {
        return get_folder( folderPath ).string();
    }
} // namespace path