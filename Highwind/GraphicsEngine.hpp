#pragma once

#include "libs.h"
#include "Shader.hpp"
#include "Camera.hpp"

class HighwindWindow;
class Shader;
class Camera;

class GraphicsEngine
{
public:
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	GraphicsEngine(HighwindWindow* window) : m_Window(window) 
	{
		std::cout << "Initializing Highwind 1.0 GraphicsEngine..." << std::endl;
	}
	~GraphicsEngine() {}

	void Initialize()
	{
		m_Camera = Camera( glm::vec3( 0.0f, 0.0f, 0.0f ) );
		glfwSetWindowUserPointer(m_Window->GetGLWindow(), this);
	}

	void GenerateShaders()
	{
		m_Shader.Compile();
	}

	

	void SetPolygonMode( int mode )
	{
		m_GLPolygonMode = mode;
	}

	void Update()
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		ProcInput();

		// todo: uncomment this
		//glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
		//glClear( GL_COLOR_BUFFER_BIT );

		m_Shader.Use();

		glm::mat4 projection = glm::perspective( glm::radians( m_Camera.Zoom ), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f );
		m_Shader.SetMat4( "projection", projection );
		glm::mat4 view = m_Camera.GetViewMatrix();
		m_Shader.SetMat4( "view", view );
	}

	void SetShader( Shader shader )
	{
		m_Shader = shader;
	}

	unsigned int GetShaderProgram()
	{
		return m_ShaderProgram;
	}

	Shader GetShader()
	{
		return m_Shader;
	}

	void ProcInput()
	{
		if (glfwGetKey( m_Window->GetGLWindow(), GLFW_KEY_ESCAPE ) == GLFW_PRESS)
		{
			if (glfwGetInputMode( m_Window->GetGLWindow(), GLFW_CURSOR ) == GLFW_CURSOR_NORMAL)
				glfwSetInputMode( m_Window->GetGLWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
			else
				glfwSetInputMode( m_Window->GetGLWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );
		}

		if (glfwGetKey( m_Window->GetGLWindow(), GLFW_KEY_W ) == GLFW_PRESS)
			m_Camera.ProcessKeyboard( FORWARD, deltaTime );
		if (glfwGetKey( m_Window->GetGLWindow(), GLFW_KEY_S ) == GLFW_PRESS)
			m_Camera.ProcessKeyboard( BACKWARD, deltaTime );
		if (glfwGetKey( m_Window->GetGLWindow(), GLFW_KEY_A ) == GLFW_PRESS)
			m_Camera.ProcessKeyboard( LEFT, deltaTime );
		if (glfwGetKey( m_Window->GetGLWindow(), GLFW_KEY_D ) == GLFW_PRESS)
			m_Camera.ProcessKeyboard( RIGHT, deltaTime );
	}

	float GetDeltaTime()
	{
		return deltaTime;
	}

	Camera GetCamera()
	{
		return m_Camera;
	}

public:
	// Cam related
	float lastX = SCR_WIDTH / 2;
	float lastY = SCR_HEIGHT / 2;
	bool firstMouse = true;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	void mouse_callback( GLFWwindow* window, double xposIn, double yposIn )
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; 

		lastX = xpos;
		lastY = ypos;

		m_Camera.ProcessMouseMovement( xoffset, yoffset );
	}

	void scroll_callback( GLFWwindow* window, double xoffset, double yoffset )
	{
		m_Camera.ProcessMouseScroll( static_cast<float> (yoffset) );
	}

	void framebuffer_size_callback( GLFWwindow* window, int width, int height )
	{
		glViewport( 0, 0, width, height );
	}

private:
	Shader m_Shader;
	Camera m_Camera;

	HighwindWindow* m_Window;
	unsigned int m_ShaderProgram;
	int m_GLPolygonMode = GL_FILL;

	std::vector<unsigned int> m_ListOfVBOs;
};

