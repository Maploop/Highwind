#pragma once

#include "libs.h"
#include <glm/glm.hpp>

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

        //m_VSCode = vertexCode.c_str();
        //m_FSCode = fragmentCode.c_str();
    }
    ~Shader() {}

    void Compile()
    {
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( vertex, 1, &m_VSCode, NULL );
        glCompileShader( vertex );
        checkCompileErrors( vertex, "VERTEX" );
        // fragment Shader
        fragment = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( fragment, 1, &m_FSCode, NULL );
        glCompileShader( fragment );
        checkCompileErrors( fragment, "FRAGMENT" );
        // shader Program
        ID = glCreateProgram();
        glAttachShader( ID, vertex );
        glAttachShader( ID, fragment );
        glLinkProgram( ID );
        checkCompileErrors( ID, "PROGRAM" );
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader( vertex );
        glDeleteShader( fragment );
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

    void SetMat4( const std::string& name, glm::mat4 &mat ) const
    {
        glUniformMatrix4fv( glGetUniformLocation( ID, name.c_str() ), 1, GL_FALSE, &mat[0][0] );
    }

private:
    const char* m_VSCode = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "\n"
        "out vec2 TexCoord;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "\n"
        "    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);\n"
        "}\0";
    const char* m_FSCode = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 ourColor;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D texture1;\n"
        "uniform sampler2D texture2;\n"
        "uniform float tex2alpha;\n"
        "void main()\n"
        "{\n"
        "    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), tex2alpha);\n"
        "}\0";

    VertexShader m_VertexShader;
    FragmentShader m_FragmentShader;

    void checkCompileErrors( unsigned int shader, std::string type )
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
            if (!success)
            {
                glGetShaderInfoLog( shader, 1024, NULL, infoLog );
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv( shader, GL_LINK_STATUS, &success );
            if (!success)
            {
                glGetProgramInfoLog( shader, 1024, NULL, infoLog );
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

