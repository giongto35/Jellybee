#pragma once

class Object;

class MiniMap
{

public:
	Object *m_map;
	Object *m_mainCharacter;

public:
	void Init();
	void Draw();
	
	// setter

private:
	~MiniMap(void);
	MiniMap(void);
};
