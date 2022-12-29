#pragma once

#include <string>  // for string

class Shaders final
{
  public:
    Shaders() = default;
    virtual ~Shaders();

    unsigned int get_id () const;
    int get_uniform_location ( std::string const & variableName ) const;

    void load ( std::string const & vertexShaderPath,
                std::string const & fragmentShaderPath );
    void use () const;

  private:
    bool         check_error ( unsigned int const & shaderValue,
                               unsigned int const & type );
    unsigned int compile ( unsigned int const & shaderType,
                           std::string const &  fileName );
    unsigned int link ( unsigned int const & vertexShader,
                        unsigned int const & fragmentShader );

    /// @brief ID of the shaders program
    unsigned int m_id;
};