#include "Framebuffer.h"
#include "ScreenResolution.h"
#include <cstring>

//---------------------------------------------------
/// Constructor
//---------------------------------------------------
Framebuffer::Framebuffer(void)
{
	m_fboId = 0;
	m_depthTextureId = 0;
	m_colorTextureId = 0;
	m_depthRenderBufferID = 0;
}

//---------------------------------------------------
/// Init
//---------------------------------------------------
void Framebuffer::Init(const char* Id)
{
	strcpy(m_Id, Id);
	GLint texWidth = ScreenResolution::GetInstance()->m_screenWidth;
	GLint texHeight = ScreenResolution::GetInstance()->m_screenHeight;

	// generate the Framebuffer, & texture object names
	glGenFramebuffers(1, &m_fboId);
	glGenTextures(1, &m_colorTextureId);
	glGenRenderbuffers(1, &(this->m_depthRenderBufferID));
	//glGenTextures(1, &m_depthTextureId);

	// bind color texture and load the texture mip-level 0
	glBindTexture(GL_TEXTURE_2D, m_colorTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

	//// bind depth texture and load the texture mip-level 0
	//// no texels need to specified as we are going to draw into 
	//// the texture
	//glBindTexture(GL_TEXTURE_2D, m_depthTextureId);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texWidth, texHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// bind the Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
	// specify texture as color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
		GL_TEXTURE_2D, m_colorTextureId, 0);
	// specify texture as depth attachment
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	//	GL_TEXTURE_2D, m_depthTextureId, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, this->m_depthRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, texWidth, texHeight);	
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->m_depthRenderBufferID);

	// check for Framebuffer complete
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		LOGE("The Framebuffer is incomplete!\n");
	}
}

//---------------------------------------------------
/// Get Ids
//---------------------------------------------------

char* Framebuffer::GetId()
{
	return m_Id;
}

GLuint Framebuffer::GetFboId()
{
	return m_fboId;
}

GLuint Framebuffer::GetColorTextureId()
{
	return m_colorTextureId;
}

GLuint Framebuffer::GetDepthTextureId()
{
	return m_depthTextureId;
}

//---------------------------------------------------
/// Destructor
//---------------------------------------------------
Framebuffer::~Framebuffer(void)
{
	if (m_fboId)
	{
		glDeleteFramebuffers(1, &m_fboId);
	}

	if (m_colorTextureId)
	{
		glDeleteTextures(1, &m_colorTextureId);
	}

	/*if (m_depthTextureId)
	{
		glDeleteTextures(1, &m_depthTextureId);
	}*/

	if (this->m_depthRenderBufferID)
	{
		glDeleteRenderbuffers(1, &(this->m_depthRenderBufferID));
	}
}
