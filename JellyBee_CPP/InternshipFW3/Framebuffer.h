#pragma once
#include "Platforms.h"

class Framebuffer
{
private:
	char m_Id[SIZE_OF_ID];
	GLuint m_fboId;
	GLuint m_depthTextureId;
	GLuint m_colorTextureId;
	GLuint m_depthRenderBufferID;

public:
	Framebuffer(void);
	~Framebuffer(void);
	void Init(const char* Id = "");
	char* GetId();
	GLuint GetFboId();
	GLuint GetColorTextureId();
	GLuint GetDepthTextureId();
};
