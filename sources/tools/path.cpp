#include "path.hpp"

#include <iostream>

#include "tools/assertion.hpp"  // for ASSERTION
#include "tools/traces.hpp"

namespace path
{
    std::filesystem::path get_folder ( E_Folder const & folderPath )
    {
        std::filesystem::path folderRelativPath {};

        switch ( folderPath )
        {
        case E_Folder::Data :
            folderRelativPath = std::filesystem::path { "data" };
            break;
        case E_Folder::Resources :
            folderRelativPath = std::filesystem::path { "resources" };
            break;
        case E_Folder::Shaders :
            folderRelativPath = get_folder( E_Folder::Resources ) / "shaders";
            break;
        case E_Folder::Tilesets :
            folderRelativPath = get_folder( E_Folder::Resources ) / "tilesets";
            break;
        case E_Folder::Character :
            folderRelativPath =
                get_folder( E_Folder::Resources ) / "characters";
            break;
        case E_Folder::Fonts :
            folderRelativPath = get_folder( E_Folder::Resources ) / "fonts";
            break;
        case E_Folder::Samples :
            folderRelativPath = get_folder( E_Folder::Resources ) / "samples";
            break;
        default :
            ASSERTION( false, "Enum not handled in switch operation" );
            break;
        }

        if ( ! std::filesystem::is_directory( folderRelativPath ) )
        {
            Trace::FileIssue( folderRelativPath );
        }

        return folderRelativPath;
    }

    std::string get_folder_str ( E_Folder const & folderPath )
    {
        return get_folder( folderPath ).string();
    }
}  // namespace path