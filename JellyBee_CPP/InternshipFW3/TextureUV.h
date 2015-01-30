#pragma once
#include "Platforms.h"

struct TextureUV
{
	char id[SIZE_OF_ID];
	float x, y; // bottom left point
	float w, h; // width height of small texture
};