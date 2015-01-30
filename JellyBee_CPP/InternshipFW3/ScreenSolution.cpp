#include "ScreenSolution.h"
#include "Platforms.h"

ScreenSolution::ScreenSolution(void)
{
	m_screenWidth = 0;
	m_screenHeight = 0;
}

ScreenSolution::~ScreenSolution(void)
{
}

void ScreenSolution::Init(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
}

float ScreenSolution::GetRatio()
{
	return (float)m_screenWidth / m_screenHeight;
}

//---------------------------------------------------
/// Get Instance
//---------------------------------------------------
ScreenSolution* ScreenSolution::s_Instance = 0;
ScreenSolution* ScreenSolution::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new ScreenSolution();
	}
	return s_Instance;
}

//---------------------------------------------------
/// Destroy Instance
//---------------------------------------------------
void ScreenSolution::Destroy() 
{
	if (s_Instance)
	{
		delete s_Instance;
		s_Instance = 0;
	}
}
