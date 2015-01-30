#pragma once
#include "object.h"
#include "Camera.h"
#include "InputManager.h"

// Singleton class
class MainCharacter :
	public Object
{
private:

	// Singleton instance
	static MainCharacter* s_Instance;

	int m_blood;
	Vector3 m_speed;
	int m_score;
	float m_magnet;

	char** m_modelID;
	float m_frameTime;
	int m_numOfModels;

	InputManager* m_inputManager;

	MainCharacter(void);
	~MainCharacter(void);

public:
	
	// Singleton handling
	static MainCharacter* GetInstance();
	static void DestroyInstance();

	// Init
	void Init(const char* file);
	void Init(
		const char* id, 
		const Vector3 & pos, 
		const Vector3 & rot,
		const Vector3 & sca, 
		Texture* texture, 
		Model* model, 
		Shader* shader, 
		int blood, 
		Vector3 & speed, 
		int score,
		const char *textureUV = NULL
	);

	// Update
	void Update();

	// Get
	int GetBlood();
	const Vector3 & GetSpeed();
	int GetScore();
	float GetMagnet();

	//Set
	void SetBlood(int blood);
	void SetSpeed(const Vector3 & speed);
	void SetScore(int score);
	void SetMagnet(float magnet);

	// Scene camera
	Camera* m_sceneCamera;
	float m_pointTime;
};
