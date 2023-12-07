#pragma once

#include "libs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    const char* vertexShaderSource = "VertexShader.vs";
    const char* fragmentShaderSource = "FragmentShader.fs";
    Shader sh = Shader( vertexShaderSource, fragmentShaderSource );
    

    GraphicsEngine engine = GraphicsEngine( &win );
    engine.SetShader( sh );
    engine.GenerateShaders();


    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
    };

    unsigned int m_VBO;
    unsigned int m_VAO;
    unsigned int m_EBO;

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glGenBuffers( 1, &m_VBO );
    glGenVertexArrays( 1, &m_VAO );
    glGenBuffers( 1, &m_EBO );

    glBindVertexArray( m_VAO );

    glBindBuffer( GL_ARRAY_BUFFER, m_VAO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    // position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (void*)0 );
    glEnableVertexAttribArray( 0 );
    // color attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ), (void*)(3 * sizeof( float )) );
    glEnableVertexAttribArray( 1 );

    unsigned int texture;
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load( "container.jpg", &width, &height, &nrChannels, 0 );
    if (data)
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data );
        glGenerateMipmap( GL_TEXTURE_2D );
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free( data );

    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), (void*)(6 * sizeof( float )) );
    glEnableVertexAttribArray( 2 );

    // Depractaed code for now!
     //engine.SetPolygonMode( GL_FILL );
     //engine.AttachToRenderer( vertices, indices );

    while (!win.ShouldClose())
    {
        engine.Update();


        glBindTexture( GL_TEXTURE_2D, texture );
        glBindVertexArray( m_VAO );
        glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwTerminate();
	return 0;
}