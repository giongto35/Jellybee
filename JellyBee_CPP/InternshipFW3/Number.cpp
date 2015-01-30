#include "Number.h"
#include "ResourcesManager.h"
#include <cstdio>

//---------------------------------------------------
/// Get Instance
//---------------------------------------------------
Number* Number::s_Instance = 0;

Number* Number::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new Number();
	}
	return s_Instance;
}

//---------------------------------------------------
/// Destroy Instance
//---------------------------------------------------
void Number::DestroyInstance() 
{
	if (s_Instance)
	{
		delete s_Instance;
		s_Instance = 0;
	}
}

//---------------------------------------------------------------------------------------------------
/// GetNumber
//---------------------------------------------------------------------------------------------------
int Number::GetNumber()
{
	return m_number;
}

//---------------------------------------------------------------------------------------------------
/// Set Position
//---------------------------------------------------------------------------------------------------
void Number::SetPosition(Vector3 position)
{
	m_position = position;
}

//---------------------------------------------------------------------------------------------------
/// SetNumber
//---------------------------------------------------------------------------------------------------
void Number::SetNumber(int n)
{
	if (m_number != n)
	{
		m_number = n;
		if (n)
		{
			SplitNumber();
		}
		else
		{
			m_nChar = 1;
			m_charArr[0] = 0;
		}
	}
}

//---------------------------------------------------------------------------------------------------
/// SplitNumber
//---------------------------------------------------------------------------------------------------
void Number::SplitNumber()
{
	int count = 0;
	int tmp = m_number;
	while (tmp)
	{
		m_charArr[count++] = tmp % 10;
		tmp /= 10;
	}
	m_nChar = count;
}

//---------------------------------------------------------------------------------------------------
/// Draw
//---------------------------------------------------------------------------------------------------
void Number::Draw()
{
	Matrix number_vp;	
	float x = m_position.x + 0.05f;
	for (int i = 0; i < m_nChar; ++i)
	{
		m_charObj[m_charArr[i]]->SetPosition(Vector3(x -= 0.05f, m_position.y, 0.0f));
		m_charObj[m_charArr[i]]->UpdateTextureUV();
		m_charObj[m_charArr[i]]->Draw(number_vp);	
	}
}

void Number::Init()
{
	if (m_charObj == NULL)
	{
		ResourcesManager* rm = ResourcesManager::GetInstance();
		m_charObj = new Object*[MAX_NUMBER_CHARACTERS];
		

		m_model = rm->GetModelById("quad");
		m_shader = rm->GetShaderById("staticObj");

		m_texture = rm->GetTextureById("bigTex");

		char textureUVid[SIZE_OF_ID];

		for (int i = 0; i < MAX_NUMBER_CHARACTERS; ++i)
		{
			m_charObj[i] = new Object();
			sprintf(textureUVid, "%d.tga", i);
			m_charObj[i]->Init("numChar", 
								Vector3(0.0f, 0.0f, 0.0f), 
								Vector3(0.0f, 0.0f, 0.0f), 
								Vector3(0.025f, 0.0415f, 1.0f), 
								m_texture,
								m_model, 
								m_shader,
								textureUVid);
		}

		m_position = Vector3(0.0f, 0.0f, 0.0f);
		m_nChar = 0;
		m_number = -1;
		SetNumber(0);
	}
}
//---------------------------------------------------------------------------------------------------
/// Constructor
//---------------------------------------------------------------------------------------------------
Number::Number()
{
	m_model = 0;
	m_shader = 0;
	m_texture = 0;
	m_charObj = 0;

	m_number = 0;
	m_nChar = 0;
}

//---------------------------------------------------------------------------------------------------
/// Destructor
//---------------------------------------------------------------------------------------------------
Number::~Number(void)
{
	for (int i = 0; i < MAX_NUMBER_CHARACTERS; ++i)
	{
		SAFE_DEL(m_charObj[i]);
	}
	SAFE_DEL(m_charObj);
}
