#pragma once
#include "State.h"
#include <vector>

class StatesManager
{
private:
	// Attributes
	static StatesManager* s_Instance;
	std::vector<State*> m_states;
public:
	State* m_currentState;

public:
	// SINGLETON HANDLING
	static StatesManager* GetInstance();
	static void DestroyInstance();

	// ACTIONS
	void Init();
	void Update();
	void SwitchState(State* newState);
	State* GetStateById(const char* stateId);
	State* GetNextState();

private:
	StatesManager(void);
	~StatesManager(void);
};
