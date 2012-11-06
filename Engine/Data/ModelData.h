#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include <vector>

#include "VertexData.h"

class ModelData
{
public:
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;
};

#endif