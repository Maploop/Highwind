#pragma once

#include "libs.h"

class VertexShader;
class FragmentShader;
class HighwindWindow;
class GraphicsEngine
{
public:
	GraphicsEngine(HighwindWindow* window) : m_Window(window) 
	{
		std::cout << "Initializing Highwind 1.0 GraphicsEngine..." << std::endl;
	}
	~GraphicsEngine() {}

	void GenerateShaders( const char* vertexShaderSource, const char* fragmentShaderSource )
	{
		VertexShader vs = VertexShader( vertexShaderSource );
		SetVertexShader( &vs );
		FragmentShader fs = FragmentShader( fragmentShaderSource );
		SetFragmentShader( &fs );

		GenerateShaders();
	}

	void GenerateShaders()
	{
		m_VertexShader->CreateShader();
		m_VertexShader->Compile();

		m_FragmentShader->CreateShader();
		m_FragmentShader->Compile();


		m_ShaderProgram = glCreateProgram();
		glAttachShader( m_ShaderProgram, m_VertexShader->GetShaderID() );
		glAttachShader( m_ShaderProgram, m_FragmentShader->GetShaderID() );

		int succ;
		char infolog[512];
		glLinkProgram( m_ShaderProgram );
		glGetProgramiv( m_ShaderProgram, GL_LINK_STATUS, &succ );
		if (!succ) {
			glGetProgramInfoLog( m_ShaderProgram, 512, NULL, infolog );
		}
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


		glUseProgram( m_ShaderProgram );
		//for (unsigned int i : m_ListOfVBOs)
		//{
		//	glBindVertexArray( i );
		//}
		//glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
		//glBindVertexArray( 0 );
	}

	void SetVertexShader( VertexShader* vs )
	{
		m_VertexShader = vs;
	}

	void SetFragmentShader( FragmentShader* fs )
	{
		m_FragmentShader = fs;
	}

	unsigned int GetShaderProgram()
	{
		return m_ShaderProgram;
	}

	VertexShader* GetVS()
	{
		return m_VertexShader;
	}

	FragmentShader* GetFS()
	{
		return m_FragmentShader;
	}

private:
	VertexShader* m_VertexShader;
	FragmentShader* m_FragmentShader;
	HighwindWindow* m_Window;
	unsigned int m_ShaderProgram;
	int m_GLPolygonMode = GL_FILL;

	std::vector<unsigned int> m_ListOfVBOs;
};

