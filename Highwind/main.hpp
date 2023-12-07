#pragma once

#include "libs.h"

void framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
    glViewport( 0, 0, width, height );
}

int main()
{
    HighwindWindow win = HighwindWindow(800, 600, "Highwind 1.0");
    win.Initialize();
    win.SetFramebufferSizeCallback( framebuffer_size_callback );
    GLFWwindow* window = win.GetGLWindow();

    GraphicsEngine engine = GraphicsEngine( &win );

    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   ourColor = aColor;\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec3 ourColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(ourColor, 1.0);\n"
        "}\0";

    engine.GenerateShaders( vertexShaderSource, fragmentShaderSource );
    int shaderProgram = engine.GetShaderProgram();


    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    unsigned int m_VBO;
    unsigned int m_VAO;
    unsigned int m_EBO;

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glGenBuffers( 1, &m_VBO );
    glGenVertexArrays( 1, &m_VAO );
    //glGenBuffers( 1, &m_EBO );

    glBindVertexArray( m_VAO );

    glBindBuffer( GL_ARRAY_BUFFER, m_VAO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    //glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_EBO );
    //glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    // Color att
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void*)0 );
    glEnableVertexAttribArray( 0 );
    // pos att
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (void*)(3 * sizeof( float )) );
    glEnableVertexAttribArray( 1 );

     engine.SetPolygonMode( GL_FILL );
     engine.AttachToRenderer( vertices, indices );

    while (!win.ShouldClose())
    {
        engine.Update();

        float timeValue = glfwGetTime();
        float greenValue = (sin( timeValue ) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation( shaderProgram, "ourColor" );
        glUniform4f( vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f );


        glBindVertexArray( m_VAO );
        glDrawArrays( GL_TRIANGLES, 0, 0 );
        glBindVertexArray( 0 );

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwTerminate();
	return 0;
}