#pragma once

#include "platforms.h"
#include "Object.h"
#include "Model.h"
#include "Vertex.h"

void GetRadiusAndCenter(Vertex *verticesData, int numOfVertices, Model *model);
bool IsHit(Object *obj1, Object *obj2);