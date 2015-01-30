#include "Texture.h"
#include "TGA.h"
#include "FileManager.h"
#include <string.h>

Texture::Texture(void)
{
	m_textureId = 0;
	m_target = 0;
	m_id[0] = '\0';

	// texture UV
	m_textureUV = 0;
	m_numOfTextureUV = 0;
}

void Texture::Init(const char* id, GLuint textureId)
{
	strcpy(m_id, id);
	m_textureId = textureId;
}

void Texture::Init(const char* file, const char *id, int target, int wrappingMode)
{
	// init id
	strcpy(m_id, id);

	// checking target = { GL_TEXTURE_2D, GL_TEXTURE_CUBE_MAP }
	if ( (target != GL_TEXTURE_2D) && (target != GL_TEXTURE_CUBE_MAP) )
	{
		LOGE("ERROR : unexpected target's value\n");
		return;
	}
	m_target = target;

	// create the OpenGL ES texture resource and get id
	glGenTextures(1, &m_textureId);
	// bind the texture to the 2D texture type
	glBindTexture(target, m_textureId);
	// create CPU buffer and load it with the image data
	int width = -1, height = -1, bpp;
	char *bufferTGA = LoadTGA(file, &width, &height, &bpp);
 
	// load the image data into OpenGL ES texture resource
	int format;
	if (bpp == 24)
    {
		format = GL_RGB;
	}
	else
	{
		format = GL_RGBA;
    }

	// target = GL_TEXTURE_2D
	if (target == GL_TEXTURE_2D)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bufferTGA);
	}
	// target = GL_TEXTURE_CUBE_MAP
	else
	{
		// get six sides of the TEXTURE_CUBE
		int w = width / 4;
		int h = height / 3;

		char* sides[6];
		sides[0] = GetPixels(bufferTGA, w * 2, h, w, h, bpp);
		sides[1] = GetPixels(bufferTGA, 0, h, w, h, bpp);
		sides[2] = GetPixels(bufferTGA, w, h * 2, w, h, bpp);
		sides[3] = GetPixels(bufferTGA, w, 0, w, h, bpp);
		sides[4] = GetPixels(bufferTGA, w, h, w, h, bpp);
		sides[5] = GetPixels(bufferTGA, w * 3, h, w, h, bpp);

		for (int i = 0; i < 6; ++i)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				format,
				w,
				h,
				0,
				format,
				GL_UNSIGNED_BYTE,
				sides[i]);

			delete[] sides[i];
		}
	}
 
	// free the client memory
	delete [] bufferTGA;
 
	//set the filters for minification and magnification
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// generate the mipmap chain
	glGenerateMipmap(target);
	//set the wrapping modes
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrappingMode);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrappingMode);

	// bind -> 0
	glBindTexture(target, 0);
}

//---------------------------------------------------
/// SETTERS
//---------------------------------------------------
void Texture::SetTextureId(GLuint id)
{
	m_textureId = id;
}

void Texture::SetTarget(GLuint target)
{
	m_target = target;
}

//---------------------------------------------------
/// GETTERS
//---------------------------------------------------
GLuint Texture::GetTextureId()
{
	return m_textureId;
}

GLuint Texture::GetTarget()
{
	return m_target;
}

const char* Texture::GetId()
{
	return m_id;
}


//---------------------------------------------------
/// TEXTURE UV GETTERS
//---------------------------------------------------
void Texture::GetTextureUvList(const char *fileName)
{
	FileManager *fileManager = FileManager::GetInstance();
	FILE *pFile = fileManager->Open(fileName, "r");

	if (pFile)
	{
		int width, height;
		fscanf(pFile, "Width: %d\n", &width);
		fscanf(pFile, "Height: %d\n", &height);
		fscanf(pFile, "Number of texture: %d\n", &m_numOfTextureUV);

		m_textureUV = new TextureUV [m_numOfTextureUV];
		char id[SIZE_OF_ID];
		int x, y, w, h;
		for (int i = 0; i < m_numOfTextureUV; i++)
		{
			fscanf(pFile, "%s %d %d %d %d\n", id, &x, &y, &w, &h);
			strcpy(m_textureUV[i].id, id);
			m_textureUV[i].w = (float) w / width;
			m_textureUV[i].h = (float) h / height;
			m_textureUV[i].x = (float) x / width;
			m_textureUV[i].y = 1 - (float) y / height - m_textureUV[i].h;
		}
	}

	fclose(pFile);
}

const TextureUV* Texture::GetTextureUvById(const char *id)
{
	for (int i = 0; i < m_numOfTextureUV; i++)
	{
		if (strcmp(id, m_textureUV[i].id) == 0)
		{
			return &m_textureUV[i];
		}
	}

	LOGI("Can't find %s in texture %s.\n", id, m_id);
	return NULL;
}


//---------------------------------------------------
/// DESTRUCTOR
//---------------------------------------------------
Texture::~Texture(void)
{
	if (m_textureId)
	{
		glDeleteTextures(1, &m_textureId);
	}

	SAFE_DEL(m_textureUV);
}
