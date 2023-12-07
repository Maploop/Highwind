#pragma once

#include "libs.h"

class FragmentShader
{
public:
	FragmentShader() {}
	FragmentShader( const char* source ) : m_Source( source ) {}
	~FragmentShader() {}

	void CreateShader()
	{
		m_FS = glCreateShader( GL_FRAGMENT_SHADER );
	}

	void Compile()
	{
		glShaderSource( m_FS, 1, &m_Source, NULL );
		glCompileShader( m_FS );

		int succ;
		char infolog[512];
		glGetShaderiv( m_FS, GL_COMPILE_STATUS, &succ );
		if (!succ)
		{
			glGetShaderInfoLog( m_FS, 512, NULL, infolog );
			std::cout << "Error While creating Fragment Shader : " << infolog << std::endl;
		}
	}

	unsigned int GetShaderID()
	{
		return m_FS;
	}

private:
	const char* m_Source;
	unsigned int m_FS;
};

