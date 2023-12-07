#pragma once

#include "libs.h"
#include "Shader.hpp"

class HighwindWindow;
class Shader;

class GraphicsEngine
{
public:
	GraphicsEngine(HighwindWindow* window) : m_Window(window) 
	{
		std::cout << "Initializing Highwind 1.0 GraphicsEngine..." << std::endl;
	}
	~GraphicsEngine() {}

	void GenerateShaders()
	{
		m_Shader.Compile();
	}

	void AttachToRenderer( float vertices[], unsigned int indices[] )
	{
		
	}

	void SetPolygonMode( int mode )
	{
		m_GLPolygonMode = mode;
	}

	void Update()
	{
		glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		m_Shader.Use();
		//for (unsigned int i : m_ListOfVBOs)
		//{
		//	glBindVertexArray( i );
		//}
		//glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
		//glBindVertexArray( 0 );
	}

	void SetShader( Shader shader )
	{
		m_Shader = shader;
	}

	unsigned int GetShaderProgram()
	{
		return m_ShaderProgram;
	}

private:
	Shader m_Shader;
	HighwindWindow* m_Window;
	unsigned int m_ShaderProgram;
	int m_GLPolygonMode = GL_FILL;

	std::vector<unsigned int> m_ListOfVBOs;
};

