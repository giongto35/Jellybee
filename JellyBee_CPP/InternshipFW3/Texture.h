#pragma once
#include "Platforms.h"
#include "TextureUV.h"

class Texture
{
private:
	GLuint m_textureId;
	GLuint m_target;
	char m_id[SIZE_OF_ID];
	TextureUV *m_textureUV;
	int m_numOfTextureUV;

public:
	Texture(void);
	~Texture(void);
	void Init(const char* file, const char *id, int target, int wrappingMode);
	void Init(const char* id, GLuint textureId);
	// setters
	void SetTextureId(GLuint id);
	void SetTarget(GLuint target);

	// getters
	GLuint GetTextureId();
	GLuint GetTarget();
	const char* GetId();

	// texture UV
	void GetTextureUvList(const char *fileName);
	const TextureUV* GetTextureUvById(const char *id);
};
