#include "ScreenResolution.h"
#include "Platforms.h"

ScreenResolution::ScreenResolution(void)
{
	m_screenWidth = 0;
	m_screenHeight = 0;
	m_densityDpi = 1;
}

ScreenResolution::~ScreenResolution(void)
{
}

void ScreenResolution::Init(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
}

float ScreenResolution::GetRatio()
{
	return (float)m_screenWidth / m_screenHeight;
}

//---------------------------------------------------
/// Get Instance
//---------------------------------------------------
ScreenResolution* ScreenResolution::s_Instance = 0;
ScreenResolution* ScreenResolution::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new ScreenResolution();
	}
	return s_Instance;
}

//---------------------------------------------------
/// Destroy Instance
//---------------------------------------------------
void ScreenResolution::DestroyInstance() 
{
	if (s_Instance)
	{
		delete s_Instance;
		s_Instance = 0;
	}
}

//---------------------------------------------------
/// Set Density
//---------------------------------------------------
void ScreenResolution::SetDensityDpi(int density)
{
	m_densityDpi = density;
}