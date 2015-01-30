#include "OBJ.h"
#include "Platforms.h"
#include "FileManager.h"
#include <Stdio.h>
#include <cstring>

void ReadOBJ(const char *fileName, Vertex *&verticesData, int &numOfVertices, Material *&material, int &numOfMaterial)
{
	FileManager *fileManager = FileManager::GetInstance();
	FILE *pFile = fileManager->Open(fileName, "r");

	if (pFile != NULL)
	{
		SKIP_ONE_LINE(pFile);

		int numOfGroup, numOfPos, numOfUv, numOfNormal, numOfFace, numOfMtl;

		Vector3 *temp_pos;
		Vector2 *temp_uv;
		Vector3 *temp_normal;

		fscanf(pFile, "#Groups: %d\n", &numOfGroup);
		MaterialData *mtlArray = NULL;

		// prepare temp_pos array
		fscanf(pFile, "#Vertices: %d\n", &numOfPos);
		temp_pos = new Vector3 [numOfPos];
		SKIP_ONE_LINE(pFile);

		// prepare temp_normal array
		fscanf(pFile, "#Normals: %d\n", &numOfNormal);
		temp_normal = new Vector3 [numOfNormal];
		SKIP_ONE_LINE(pFile);

		// prepare temp_uv array
		fscanf(pFile, "#TexCoords: %d\n", &numOfUv);
		temp_uv = new Vector2 [numOfUv];
		SKIP_ONE_LINE(pFile);

		fscanf(pFile, "#Faces: %d\n", &numOfFace);
		SKIP_ONE_LINE(pFile);

		//---------------------------------------------
		char line[256];
		char mtlFileName[256];
		int vCount = 0, vtCount = 0, vnCount = 0, fCount = 0, verticesCount = 0, mtlCount = 0;
		int vIndex[3], vtIndex[3], vnIndex[3];

		Vector3 v, vn;
		Vector2 vt;

		int format;
		if (numOfUv != 0 && numOfNormal != 0)
		{
			format = 1;
		}
		else if (numOfUv != 0 && numOfNormal == 0)
		{
			format = 2;
		}

		// prepare verticesData[], material[]
		numOfVertices = numOfFace * 3;
		verticesData = new Vertex[numOfVertices];
		numOfMaterial = numOfGroup;
		material = new Material [numOfMaterial];

		do
		{
			fscanf(pFile, "%s", line);

			if (strcmp(line, "#") == 0) // comment line start with #
			{
				SKIP_ONE_LINE(pFile);
			}

			else if (strcmp(line, "mtllib") == 0)
			{
				fscanf(pFile, "%s\n", mtlFileName);
				ReadMTL(mtlFileName, mtlArray, numOfMtl);
			}

			else if (strcmp(line, "v") == 0)

			{
				fscanf(pFile, "%f %f %f\n", &v.x, &v.y, &v.z);
				temp_pos[vCount] = v;
				vCount++;
			}

			else if (strcmp(line, "vt") == 0)
			{
				fscanf(pFile, "%f %f\n", &vt.x, &vt.y);
				temp_uv[vtCount] = vt;
				vtCount++;
			}

			else if (strcmp(line, "vn") == 0)
			{
				fscanf(pFile, "%f %f %f\n", &vn.x, &vn.y, &vn.z);
				temp_normal[vnCount] = vn;
				vnCount++;
			}

			else if (strcmp(line, "usemtl") == 0)
			{
				fscanf(pFile, "%s\n", line);
				strcpy(material[mtlCount].textureUvId, GetTextureFilenameFromMtlById(mtlArray, numOfMtl, line));
				material[mtlCount].offset = verticesCount;
				if (mtlCount > 0)
				{
					material[mtlCount - 1].length = verticesCount - material[mtlCount - 1].offset;
				}
				mtlCount++;
			}

			else if (strcmp(line, "f") == 0)
			{
				// format 1 v/vt/vn
				if (format == 1)
				{
					fscanf(pFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
						&vIndex[0], &vtIndex[0], &vnIndex[0], 
						&vIndex[1], &vtIndex[1], &vnIndex[1], 
						&vIndex[2], &vtIndex[2], &vnIndex[2]);
					
					for (int i = 0; i < 3; i++)
					{
						verticesData[verticesCount].pos = temp_pos[vIndex[i] - 1];
						verticesData[verticesCount].uv = temp_uv[vtIndex[i] - 1];
						verticesData[verticesCount].normal = temp_normal[vnIndex[i] - 1];
						verticesCount++;
					}
				}

				// format 2 v/vt
				else if (format == 2)
				{
					fscanf(pFile, "%d/%d %d/%d %d/%d\n", 
						&vIndex[0], &vtIndex[0], 
						&vIndex[1], &vtIndex[1],
						&vIndex[2], &vtIndex[2]);
					
					for (int i = 0; i < 3; i++)
					{
						verticesData[verticesCount].pos = temp_pos[vIndex[i] - 1];
						verticesData[verticesCount].uv = temp_uv[vtIndex[i] - 1];
						verticesCount++;
					}
				}
				
				fCount++;
			}

			else
			{
				SKIP_ONE_LINE(pFile);
			}
		}
		while (!feof(pFile));

		if (vCount == numOfPos && vtCount == numOfUv && vnCount == numOfNormal && verticesCount == numOfVertices)
		{
			printf("Loading %s OK size = %d byte\n", fileName, numOfVertices * sizeof(Vertex));
			material[mtlCount - 1].length = verticesCount - material[mtlCount - 1].offset;
		}

		fclose(pFile);

		SAFE_DEL(temp_pos);
		SAFE_DEL(temp_uv);
		SAFE_DEL(temp_normal);
		SAFE_DEL(mtlArray);
	}
}

void ReadMTL(const char *fileName, MaterialData *&mtlArray, int &numOfMtlData)
{
	FileManager *fileManager = FileManager::GetInstance();
	char fullPath[256];
	sprintf(fullPath, "Models/%s", fileName);
	FILE *pFile = fileManager->Open(fullPath, "r");

	if (pFile != NULL)
	{

		SKIP_ONE_LINE(pFile);

		int mtlCount = 0;
		fscanf(pFile, "# Material Count: %d\n", &numOfMtlData);
		mtlArray = new MaterialData [numOfMtlData];

		char line[256];

		do
		{
			fscanf(pFile, "%s", line);

			if (strcmp(line, "newmtl") == 0)
			{
				fscanf(pFile, "%s\n", line);
				strcpy(mtlArray[mtlCount].id, line);
			}

			else if (strcmp(line, "map_Kd") == 0)
			{
				fscanf(pFile, "%s\n", line);
				strcpy(mtlArray[mtlCount].textureUvId, line);
				mtlCount++;
			}

			else // if not start with "newmtl" or "map_Kd" then skip
			{
				SKIP_ONE_LINE(pFile);
			}
		}
		while (!feof(pFile));
	
		fclose(pFile);
	}
}

char *GetTextureFilenameFromMtlById(MaterialData *mtlData, int numOfMtlData, const char *id)
{
	for (int i = 0; i < numOfMtlData; i++)
	{
		if (strcmp(mtlData[i].id, id) == 0)
		{
			return mtlData[i].textureUvId;
		}
	}
	LOGI("Cannot find %s in mtlData.\n", id);
	return NULL;
}
