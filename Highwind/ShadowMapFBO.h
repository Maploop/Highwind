#pragma once

#include <GL/glew.h>
#include "logger.hpp"

class ShadowMapFBO
{
public:
	ShadowMapFBO() {}
	~ShadowMapFBO() {}

	bool initialize(unsigned int width, unsigned int height)
	{
		m_width = width;
		m_height = height;

		glGenFramebuffers(1, &m_fbo);

		glGenTextures(1, &m_shadow_map);
		glBindTexture(GL_TEXTURE_2D, m_shadow_map);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadow_map, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			FERROR("FB Error: Status: %s", status);
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}

	void bind_for_writing()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glViewport(0, 0, m_width, m_height);
	}

	void bind_for_reading(GLenum texture_unit)
	{
		glActiveTexture(texture_unit);
		glBindTexture(GL_TEXTURE_2D, m_shadow_map);
	}

private:
	unsigned int m_width = 0;
	unsigned int m_height = 0;
	GLuint m_fbo;
	GLuint m_shadow_map;
};