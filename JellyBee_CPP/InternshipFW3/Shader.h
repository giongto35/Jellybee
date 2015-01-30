#pragma once
#include "Platforms.h"
#include <vector>
class Shader 
{
public:
	char m_id[SIZE_OF_ID];
	GLuint program, vertexShader, fragmentShader;
	std::vector<GLenum> states;

	GLint a_position;
	GLint u_color;
	GLint u_light;
	GLint u_campos;
	GLint a_uv;
	GLint u_lightMatWVP;
	GLint u_MatWVP;
	GLint u_MatW;
	GLint u_tex[5];
	GLint a_normal;
	GLint u_time;
	GLint u_data;
	GLint u_otherdata;
	// texture cube
	GLint a_CubeVertexPos;
	GLint u_CubeMVPMatrix;
	GLint u_samplerCubeMap;

	//Method
	Shader();
	GLuint LoadProgram ( GLuint vertexShader, GLuint fragmentShader ) ;
	GLuint LoadShader ( GLenum type, const char * filename ) ;
	int Init(const char* id, const char* vertexShaderPath, const char* fragmentShaderPath);
	virtual ~Shader();
	void AddState(GLenum state);
	void EnableState();
	void DisableState();
	const char* GetId();
};
