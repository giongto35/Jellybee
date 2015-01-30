#include "Map.h"
#include <stdio.h>      /* printf */
#include <string.h>
#include <math.h>

void Map::LoadMap(char* ID, char* file)
{
	strcpy(this->m_id, ID);
	int bpp;
	char *bufferTGA = LoadTGA(file, &m_width, &m_height, &bpp);
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
		{
			unsigned char r = bufferTGA[j * 3 + i * m_width * 3];
			unsigned char g = bufferTGA[j * 3 + 1 + i * m_width * 3];
			unsigned char b = bufferTGA[j * 3 + 2 + i * m_width * 3];

			// BLOCK
			if (r == 0 && g == 0 && b == 0)
			{
				m_map[i][j] = BLOCK;
			}			

			// TREE
			else if (r == 0 && g == 255 && b == 0)
			{
				m_map[i][j] = TREE;
			}

			// START - PATH
			else if (r == 0 && g == 0 && b == 255)
			{
				m_start = Vector2((float)i,(float)j);
				LOGI("Start pos: (%d, %d)\n", i, j);
				m_map[i][j] = PATH;
			}

			// HONEY
			else if (r == 255 && g == 255 && b == 0)
			{
				++m_nItems;
				m_map[i][j] = HONEY;
			}

			// HONEY FAKE
			else if (r == 255 && g == 0 && b == 255)
			{
				++m_nItems;
				m_map[i][j] = HONEY_FAKE;
			}

			// HEART
			else if (r == 255 && g == 0 && b == 100)
			{
				++m_nItems;
				m_map[i][j] = HEART;
			}

			// MOUNTAIN DEW
			else if (r == 0 && g == 255 && b == 255)
			{
				++m_nItems;
				m_map[i][j] = MOUNTAIN_DEW;
			}

			// WEB
			else if (r == 0 && g == 100 && b == 255)
			{
				++m_nItems;
				m_map[i][j] = SPIDER_WEB;
			}

			// END
			else if (r == 255 && g == 0 && b == 0)
			{
				++m_nItems;
				m_map[i][j] = END;
			}

			// TELEPORT GATE
			else if (r == 100 && g == 100 && b >= 199 && b <= 200)
			{
				++m_nItems;
				if (b == 199)
				{
					m_map[i][j] = TELE1;
					m_tele1.port[m_tele1.n++] = Vector2((float)i, (float)j);
				}
				else if (b == 200)
				{
					m_map[i][j] = TELE2;
					m_tele2.port[m_tele2.n++] = Vector2((float)i, (float)j);
				}
			}

			// PATH
			else // if (r == 255 && g == 255 && b == 255)
			{
				m_map[i][j] = PATH;
			}
		}

	for (int i = 0; i < m_height; i++)
	for (int j = 0; j < m_width; j++)
	{
		if (m_map[i][j] != BLOCK && m_map[i][j] != TREE)
		{
			for (int _i = i-1; _i <= i+1; ++_i)
			for (int _j = j-1; _j <= j+1; ++_j)
			{
				if (m_map[_i][_j] == BLOCK)
				{
					m_map[_i][_j] = TREE;
				}
			}
		}
	}

	SAFE_DEL(bufferTGA);
}

int Map::GetHeight()
{
	return m_height;
}

int Map::GetWidth()
{
	return m_width;
}

MapType Map::GetPoint(int y, int x)
{
	return m_map[y][x];
}

int Round(double number)
{
    return (int) (number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5));
}

MapType Map::GetFromFloatPoint(Vector2 v)
{
	return GetFromFloatPoint(v.x, v.y);
}

Vector2 Map::GetMapPos(float z, float x)
{
	return Vector2((float)Round(-z / MAPFACTOR), (float)Round(x / MAPFACTOR + m_width/2.0));
}

MapType Map::GetFromFloatPoint(float z, float x)
{
	return m_map[Round(-z / MAPFACTOR)][Round(x / MAPFACTOR + m_width/2.0)];
}

Vector2 Map::ToPoint(float z, float x)
{
	return Vector2((float)(int)(z),(float)(int)(x));
}

char* Map::GetId()
{
	return m_id;
}

Map::Map(void)
{
	m_nItems = 0;
	m_tele1.n = 0;
	m_tele2.n = 0;
}

Map::~Map(void)
{
}

Vector2 Map::Tele1(Vector2 currentPort)
{
	if (currentPort.x == m_tele1.port[0].x && currentPort.y == m_tele1.port[0].y)
	{
		return m_tele1.port[1];
	}
	return m_tele1.port[0];
}

Vector2 Map::Tele2(Vector2 currentPort)
{
	if (currentPort.x == m_tele2.port[0].x && currentPort.y == m_tele2.port[0].y)
	{
		return m_tele2.port[1];
	}
	return m_tele2.port[0];
}