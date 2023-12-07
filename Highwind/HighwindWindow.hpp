#pragma once

#include "libs.h"

class HighwindWindow
{
public:
	HighwindWindow( int width, int height, const char* title ) : m_Width(width), m_Height(height), m_WinTitle(title)
	{
		m_Window = NULL;
	}

	~HighwindWindow()
	{

	}

	void Initialize()
	{
		glfwInit();
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		GLFWwindow* window = glfwCreateWindow( m_Width, m_Height, m_WinTitle, NULL, NULL );
		if (window == NULL)
		{
			std::cout << "An error has occurred while creating the window." << std::endl;
			glfwTerminate();
			return;
		}
		m_Window = window;

		glfwMakeContextCurrent( window );
		if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		glViewport( 0, 0, m_Width, m_Height );
	}

	bool ShouldClose()
	{
		return glfwWindowShouldClose( m_Window );
	}

	GLFWwindow* GetGLWindow()
	{
		return m_Window;
	}

	void SetFramebufferSizeCallback( GLFWframebuffersizefun callb )
	{
		glfwSetFramebufferSizeCallback( m_Window, callb );
	}

private:
	int m_Width;
	int m_Height;
	GLFWwindow* m_Window;
	const char* m_WinTitle;

private:
	void framebuffer_size_callback( GLFWwindow* window, int width, int height )
	{
		glViewport( 0, 0, width, height );
	}
};

