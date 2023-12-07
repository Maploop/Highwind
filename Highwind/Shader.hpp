#pragma once

#include "libs.h"

class VertexShader;
class FragmentShader;

class Shader
{
public:
    unsigned int ID;

    Shader() {}
    Shader( const char* vertexPath, const char* fragmentPath )
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        fShaderFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
        try
        {

            vShaderFile.open( vertexPath );
            fShaderFile.open( fragmentPath );
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        m_VSCode = vertexCode.c_str();
        m_FSCode = fragmentCode.c_str();

        m_VertexShader = VertexShader( m_VSCode );
        m_FragmentShader = FragmentShader( m_FSCode );
    }
    ~Shader() {}

    void Compile()
    {
        m_VertexShader.Compile();
        m_FragmentShader.Compile();

        int success;
        char infoLog[512];

        ID = glCreateProgram();
        glAttachShader( ID, m_VertexShader.GetShaderID() );
        glAttachShader( ID, m_FragmentShader.GetShaderID() );
        glLinkProgram( ID );

        glGetProgramiv( ID, GL_LINK_STATUS, &success );
        if (!success)
        {
            glGetProgramInfoLog( ID, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader( m_VertexShader.GetShaderID() );
        glDeleteShader( m_FragmentShader.GetShaderID() );
    }

    void Use()
    {
        glUseProgram( ID );
    }

    void SetBool( const std::string& name, bool value ) const
    {
        glUniform1i( glGetUniformLocation( ID, name.c_str() ), (int)value );
    }

    void SetInt( const std::string& name, int value ) const
    {
        glUniform1i( glGetUniformLocation( ID, name.c_str() ), value );
    }

    void SetFloat( const std::string& name, float value ) const
    {
        glUniform1f( glGetUniformLocation( ID, name.c_str() ), value );
    }

    void Set4F( const std::string& name, float v1, float v2, float v3, float v4 )
    {
        glUniform4f( glGetUniformLocation( ID, name.c_str() ), v1, v2, v3, v4 );
    }

private:
    const char* m_VSCode;
    const char* m_FSCode;

    VertexShader m_VertexShader;
    FragmentShader m_FragmentShader;
};

