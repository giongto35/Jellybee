#include "Collision.h"
#include "ScenesManager.h"
#include "Camera.h"
#include "math.h"

void GetRadiusAndCenter(Vertex *verticesData, int numOfVertices, Model *model)
{
	float minX = verticesData[0].pos.x;
	float minY = verticesData[0].pos.y;
	float minZ = verticesData[0].pos.z;
	float maxX = verticesData[0].pos.x;
	float maxY = verticesData[0].pos.y;
	float maxZ = verticesData[0].pos.z;

	for (int i = 1; i < numOfVertices; i++)
	{
		if (verticesData[i].pos.x < minX)
		{
			minX = verticesData[i].pos.x;
		}
		else if (verticesData[i].pos.x > maxX)
		{
			maxX = verticesData[i].pos.x;
		}

		if (verticesData[i].pos.y < minY)
		{
			minY = verticesData[i].pos.y;
		}
		else if (verticesData[i].pos.y > maxY)
		{
			maxY = verticesData[i].pos.y;
		}

		if (verticesData[i].pos.z < minZ)
		{
			minZ = verticesData[i].pos.z;
		}
		else if (verticesData[i].pos.z > maxZ)
		{
			maxZ = verticesData[i].pos.z;
		}
	}

	float delX = fabs(maxX - minX);
	float delY = fabs(maxY - minY);
	float delZ = fabs(maxZ - minZ);

	LOGI("Model %s hinh hop %f hinh cau %f\n", model->GetId(), delX * delY * delZ, 4 * PI * pow(delY/2, 3) / 3);

	model->m_center.x = delX/2 + minX;
	model->m_center.y = delY/2 + minY;
	model->m_center.z = delZ/2 + minZ;

	float radius = delX;

	if (delY > radius)
	{
		radius = delY;
	}

	if (delZ > radius)
	{
		radius = delZ;
	}
	
	model->m_radius = radius / 2;

	/*
	model->m_radiusX = delX / 2;
	model->m_radiusY = delY / 2;
	model->m_radiusZ = delZ / 2;
	*/
}

bool IsHit(Object *obj1, Object *obj2)
{
	bool isHit = 0;
	
	float distance;

	//world matrix
	Vector4 moveL = Vector4(0, 0, -0.5, 1);
	Vector4 MainCharacterPosition;
	
	MainCharacterPosition = moveL  * ScenesManager::GetInstance()->GetCameraById("sceneCam")->CalculateWorldMatrix();
	obj1->m_center = MainCharacterPosition.xyz;

	distance = sqrt((obj1->m_center.x - obj2->m_center.x) * (obj1->m_center.x - obj2->m_center.x)
		+ (obj1->m_center.y - obj2->m_center.y) * (obj1->m_center.y - obj2->m_center.y)
		+ (obj1->m_center.z - obj2->m_center.z) * (obj1->m_center.z - obj2->m_center.z));

	//LOGI("Distance: %f\n", distance);
	if (distance < (obj1->m_radius + obj2->m_radius))
	{
		isHit = 1;
	}

	/*
	Vector3 distance;
	distance.x = fabs(obj1->m_center.x - obj2->m_center.x);
	distance.y = fabs(obj1->m_center.y - obj2->m_center.y);
	distance.z = fabs(obj1->m_center.z - obj2->m_center.z);

	if ((distance.x < (obj1->m_radiusX + obj2->m_radiusX))
		&& (distance.y < (obj1->m_radiusY + obj2->m_radiusY))
		&& (distance.z < (obj1->m_radiusZ + obj2->m_radiusZ)))
	{
		isHit = 1;
	}
	*/

	return isHit;
}