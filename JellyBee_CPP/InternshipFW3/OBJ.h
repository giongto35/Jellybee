#pragma once
#include "Vertex.h"
#include "MaterialStruct.h"

void ReadOBJ(const char *fileName, Vertex *&verticesData, int &numOfVertices, Material *&material, int &numOfMaterial);
void ReadMTL(const char *fileName, MaterialData *&mtlArray, int &numOfMtlData);
char *GetTextureFilenameFromMtlById(MaterialData *mtlData, int numOfMtlData, const char *id);
