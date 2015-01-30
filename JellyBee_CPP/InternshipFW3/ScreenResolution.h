#pragma once

class ScreenResolution
{
public:
	int m_screenWidth;
	int m_screenHeight;
	int m_densityDpi;

private:
	ScreenResolution(void);
	~ScreenResolution(void);
	static ScreenResolution* s_Instance;
public:
	void Init(int width, int height);	
	static ScreenResolution* GetInstance();
	static void DestroyInstance();
	float GetRatio();
	void SetDensityDpi(int density);
};
