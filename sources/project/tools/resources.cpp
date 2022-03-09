#include "resources.hpp"

// TYPO Mettre plein de truc dans ce namespace
namespace too
{
    std::string get_resources_path() { return "./resources"s; }
    std::string get_shaders_path()
    {
        return too::get_resources_path() + "/shaders"s;
    }
    std::string get_databases_path()
    {
        return too::get_resources_path() + "/databases"s;
    }
} // namespace too