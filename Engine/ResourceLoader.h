#ifndef _RESOURCELOADER_H_
#define _RESOURCELOADER_H_

#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <memory>

#include "Data\ModelData.h"
#include "Data\VertexData.h"

using namespace std;

class ResourceLoader
{
public:
	ResourceLoader();
	ResourceLoader(const ResourceLoader&);
	~ResourceLoader();

	std::shared_ptr<ModelData> LoadModelData(char* fileName);

private:
	map<char*, std::shared_ptr<ModelData>> m_ModelData;
};

#endif