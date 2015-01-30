#pragma once

class ScreenSolution
{
public:
	int m_screenWidth;
	int m_screenHeight;

private:
	ScreenSolution(void);
	~ScreenSolution(void);
	static ScreenSolution* s_Instance;
public:
	void Init(int width, int height);	
	static ScreenSolution* GetInstance();
	static void Destroy();
	float GetRatio();
};
