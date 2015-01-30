#include "Shader.h"
#include <cstring>
#include "FileManager.h"

Shader::Shader()
{
	m_id[0] = '\0';

	states.clear();

	a_position = -1;
	a_normal = -1;
	a_uv = -1;
	u_color = -1;
	u_MatWVP = -1;
	u_MatW = -1;
	u_campos = -1;
	u_light = -1;
	u_lightMatWVP = -1;
	for (int i = 0; i < MAX_TEX; i++)
	u_tex[i] = -1;
	u_otherdata = -1;
	u_data = -1;

	// cube
	a_CubeVertexPos = -1;
	u_CubeMVPMatrix = -1;
	u_samplerCubeMap = -1;

	program = -1;
	vertexShader = -1;
	fragmentShader = -1;
}

int Shader::Init(const char* id, const char* vertexShaderPath, const char* fragmentShaderPath)
{
	// init id
	strcpy(m_id, id);

	vertexShader = LoadShader(GL_VERTEX_SHADER, vertexShaderPath);
	if ( vertexShader == 0 )
	{
		LOGI("\nLoading %s fail", vertexShaderPath);
		return -1;
	}
	fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentShaderPath);

	if ( fragmentShader == 0 )
	{
		LOGI("\nLoading %s fail", fragmentShaderPath);
		glDeleteShader( vertexShader );
		return -2;
	}
	program = LoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes

	a_position = glGetAttribLocation(program, "a_position");
	a_normal = glGetAttribLocation(program, "a_normal");
	a_uv = glGetAttribLocation(program, "a_uv");
	u_color = glGetUniformLocation(program, "u_color");
	u_light = glGetUniformLocation(program, "u_light");
	u_time = glGetUniformLocation(program, "u_time");
	u_lightMatWVP = glGetUniformLocation(program, "u_lightMatWVP");
	u_MatWVP = glGetUniformLocation(program, "u_MatWVP");
	u_MatW = glGetUniformLocation(program, "u_MatW");
	u_campos = glGetUniformLocation(program, "u_campos");
	u_tex[0] = glGetUniformLocation(program, "u_texture");
	u_data = glGetUniformLocation(program, "u_data");
	u_otherdata = glGetUniformLocation(program, "u_otherdata");
	for (int i = 1; i < MAX_TEX; i++)
	{
		char s[20];
		sprintf(s, "u_texture%d", i);
		u_tex[i] = glGetUniformLocation(program, s);
	}

	// cube
	a_CubeVertexPos = glGetAttribLocation(program, "a_CubeVertexPos");
	u_CubeMVPMatrix = glGetUniformLocation(program, "u_CubeMVPMatrix");
	u_samplerCubeMap = glGetUniformLocation(program, "u_samplerCubeMap");
	
	return true;
}
void Shader::AddState(GLenum state)
{
	states.push_back( state );
}
void Shader::EnableState()
{
	for( GLuint i = 0; i < states.size(); i++ )
	{
		glEnable( states[i] );
		if ( states[i] == GL_BLEND )
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
	}
}
void Shader::DisableState()
{
	for( GLuint i = 0; i < states.size(); i++ )
	{
		glDisable( states[i] );
	}
}
/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param Shaderrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint Shader::LoadShader ( GLenum type, const char * filename )
{
	GLuint shader;
	GLint compiled;

	// Create the shader object
	shader = glCreateShader ( type );

	if ( shader == 0 )
	return 0;
	
	FILE * pf = FileManager::GetInstance()->Open(filename, "rb");
	if (pf == 0)
	{
		return 0;
	}
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * buffer = new char[size + 1];
	fread(buffer, sizeof(char), size, pf);
	buffer[size] = 0;
	fclose(pf);

	glShaderSource ( shader, 1, (const char **)&buffer, NULL );
	delete [] buffer;
	buffer = NULL;
	// Compile the shader
	glCompileShader ( shader );

	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

	if ( !compiled ) 
	{
		GLint infoLen = 0;

		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char  [infoLen];

			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			LOGI ( "\nError compiling shader:\n%s\n", infoLog );            

			delete [] infoLog;
		}

		glDeleteShader ( shader );
		return 0;
	}
	return shader;
}


GLuint Shader::LoadProgram ( GLuint vertexShader, GLuint fragmentShader )
{
	GLuint programObject;
	GLint linked;

	// Create the program object
	programObject = glCreateProgram ( );

	if ( programObject == 0 )
		return 0;

	glAttachShader ( programObject, vertexShader );
	glAttachShader ( programObject, fragmentShader );

	// Link the program
	glLinkProgram ( programObject );

	// Check the link status
	glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

	if ( !linked ) 
	{
		GLint infoLen = 0;

		glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = new char[sizeof(char) * infoLen];

			glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
			LOGI ( "\nError linking program:\n%s\n", infoLog );            

			delete infoLog;
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	return programObject;
}
Shader::~Shader()
{
	if ( program )
	{
		glDeleteProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
}

const char* Shader::GetId()
{
	return m_id;
}
