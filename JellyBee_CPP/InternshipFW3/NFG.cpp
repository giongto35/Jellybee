#include "NFG.h"
#include "FileManager.h"

void LoadNFG(const char * szFileName, int & vertexNum, Vertex *& vertexData, int & indexNum, unsigned short *& indices)
{
	FILE* f = FileManager::GetInstance()->Open(szFileName, "r");

	if (f == 0)
        return;

	// get vertexData
	fscanf(f, "NrVertices: %d\n", &vertexNum);
	vertexData = new Vertex[vertexNum];
	for(int i = 0; i < vertexNum; ++i)
	{
		fscanf(f, "%*d. pos:[%f, %f, %f]; ", &vertexData[i].pos.x, &vertexData[i].pos.y, &vertexData[i].pos.z);
		fscanf(f, "norm:[%f, %f, %f]; ", &vertexData[i].normal.x, &vertexData[i].normal.y, &vertexData[i].normal.z);
		fscanf(f, "binorm:[%f, %f, %f]; ", &vertexData[i].binormal.x, &vertexData[i].binormal.y, &vertexData[i].binormal.z);
		fscanf(f, "tgt:[%f, %f, %f]; ", &vertexData[i].tangent.x, &vertexData[i].tangent.y, &vertexData[i].tangent.z);
		fscanf(f, "uv:[%f, %f];\n", &vertexData[i].uv.x, &vertexData[i].uv.y);
	}

	// get indices
	fscanf(f, "NrIndices: %d\n", &indexNum);
	indices = new unsigned short[indexNum];
	for(int i = 0; i < indexNum; i += 3)
	{
		fscanf(f, "%*d. %hu, %hu, %hu\n", &indices[i], &indices[i + 1], &indices[i + 2]);	
	}

	fclose(f);

}