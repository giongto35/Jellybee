#pragma once
#include "State.h"

class Logo : public State
{
	static Logo *s_Instance;

	Logo(void);
	~Logo(void);

public:
	static Logo* GetInstance();
	static void DestroyInstance();

	void Start();
	void Update();
	void Draw();
};
