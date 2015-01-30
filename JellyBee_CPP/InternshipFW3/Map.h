#pragma once
#include "Platforms.h"
#include "Vector4.h"
#include "TGA.h"

class Map
{
private:
	MapType m_map[200][200];
	char m_id[MAX_10];
	int m_width;
	int m_height;
public:
	int m_nItems;

	struct 
	{
		Vector2 port[2];
		int n;
	} m_tele1, m_tele2;

	Vector2 Tele1(Vector2 currentPort);
	Vector2 Tele2(Vector2 currentPort);

	Vector2 m_start;
	int GetWidth();
	int GetHeight();
	void LoadMap(char* ID, char* file);
	Vector2 GetMapPos(float z, float x);
	MapType GetPoint(int y, int x);
	MapType GetFromFloatPoint(Vector2 v);
	MapType GetFromFloatPoint(float z, float x);
	Vector2 ToPoint(float y, float x);
	char* GetId();
	Map(void);
	~Map(void);
};
