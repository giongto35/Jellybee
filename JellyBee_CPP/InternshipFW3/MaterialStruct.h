#pragma once
#include "Platforms.h"

struct MaterialData
{
	char id[MAX_256];
	char textureUvId[MAX_256];
};

struct Material
{
	char textureUvId[MAX_256];
	int offset; // begin
	int length;
};