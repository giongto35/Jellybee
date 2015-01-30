#pragma once
#include "matrix.h"
#include "TextureUV.h"

class Shader;
class Model;
class Texture;

class Object
{
public:
	Vector3 m_position, m_rotation, m_scale;
	bool m_isVisible;
protected:
	Texture** m_textures;
	Model* m_model;
	Shader* m_shader;
	Matrix m_worldMatrix;
	char m_id[MAX_10];
	int m_NumTex;
public:
	Vector3 m_center;
	float m_radius;
	float m_radiusX;
	float m_radiusY;
	float m_radiusZ;
	TextureUV *m_textureUV;

	float m_otherData;

public:
	Object(void);
	virtual ~Object(void);
	virtual void Init(const char * id, const Vector3 & pos, const Vector3 & rot, const Vector3 & sca, int num_tex, Texture** texture, Model* model, Shader* shader);
	virtual void Init(const char* id, const Vector3 & pos, const Vector3 & rot, const Vector3 & sca, Texture* texture, Model* model, Shader* shader, const char *textureUvId = NULL);
	virtual void Draw(Matrix & vpMatrix, Vector3 ucolor = Vector3(1.0f, 1.0f, 1.0f));
	virtual void Update();
	virtual void Draw(Matrix & vpMatrix, Vector4 & color);

	// Setters
	void SetPosition(const Vector3 & pos);
	void SetRotation(const Vector3 & rot);
	void SetScale(const Vector3 & sca);
	void SetTexture(Texture * texture);
	void SetModel(Model * model);
	void SetShader(Shader * shader);
	void SetWorldMatrix(const Matrix & wMat);
	void SetOtherData(float otherData);

	// Getters
	const Vector3 & GetPosition();
	const Vector3 & GetRotation();
	const Vector3 & GetScale();
	const Matrix & GetWorldMatrix();
	Texture * GetTexture();
	Model * GetModel();
	Shader * GetShader();
	const char* GetId();

	void CalculateCenter();

	void SetTextureUV(const char *textureUvId = NULL);
	void UpdateTextureUV();

protected:
	void CalculateWorldMatrix();
};
