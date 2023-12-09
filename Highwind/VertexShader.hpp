#pragma once

#include "libs.h"

class VertexShader
{
public:
	VertexShader() {}
	VertexShader( const char* source ) : m_Source( source ) {}
	~VertexShader() {}

	void CreateShader()
	{
		m_VS = glCreateShader( GL_VERTEX_SHADER );
	}

	void Compile()
	{
		glShaderSource( m_VS, 1, &m_Source, NULL );
		glCompileShader( m_VS );

		int succ;
		char infolog[512];
		glGetShaderiv( m_VS, GL_COMPILE_STATUS, &succ );
		if (!succ)
		{
			glGetShaderInfoLog( m_VS, 512, NULL, infolog );
			std::cout << "Error While creating Vertex Shader : " << infolog << std::endl;
		}
		std::cout << "Shader: " << m_VS << std::endl;
	}

	unsigned int GetShaderID()
	{
		return m_VS;
	}

private:
	const char* m_Source;
	unsigned int m_VS;
};

