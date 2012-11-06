#include "ResourceLoader.h"

//GLOBALS
vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    return split(s, delim, elems);
}

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::ResourceLoader(const ResourceLoader&)
{
}

ResourceLoader::~ResourceLoader()
{
}

bool ResourceLoader::Initialize()
{
	return true;
}

bool ResourceLoader::Shutdown()
{
	return true;
}

std::shared_ptr<ModelData> ResourceLoader::LoadModelData(char* filename)
{
	if (this->m_ModelData.find(filename) == this->m_ModelData.end())
	{
		std::ifstream infile(filename);
	
		string line = "";

		vector<Vertex> vertices;
		vector<Vertex> textureCoords;
		vector<Vertex> normals;
		std::map<string, int> createdVertices;
		std::shared_ptr<ModelData> data(new ModelData);

		float x,y,z;
		int index = 0;
		while (std::getline(infile, line))
		{
			if( line.length() && line[0] != '#') {
			
				std::istringstream iss(line);

				string type;
				iss>>type;
				if(type == "v" || type == "vt" || type == "vn")
				{
					if (!(iss >> x >> y >> z)) 
						break; 
					Vertex v(x,y,z);
				
					if(type == "v")
					{
						//VertexData vertex;
						//vertex.position = v;
						//data.vertices.push_back(vertex);					
						vertices.push_back(v);
					}
					else if(type == "vt")
						textureCoords.push_back(Vertex(v.x,v.y,v.z));
					else if(type == "vn")
						normals.push_back(v);
				}
				else if(type == "f") 
				{
					string v[3];
					if (!(iss >> v[0] >> v[1] >> v[2])) 
						break; 

					//f 12/22/12 7/15/7 15/32/15

					for (int i = 0; i < 3; i++)
					{
						vector<string> splits = split(v[i],'/');
						int vindex;
						int tindex;
						int nindex;
						
						stringstream(splits[0]) >> vindex;
						stringstream(splits[1]) >> tindex;
						stringstream(splits[2]) >> nindex;

						VertexData newVertex;
						string vertNumber = std::to_string(static_cast<long long>(vindex)) + "/" + std::to_string(static_cast<long long>(tindex)) + "/" +  std::to_string(static_cast<long long>(nindex));
						if (createdVertices.find(vertNumber) == createdVertices.end())
						{
							newVertex.position = vertices[vindex-1];
							newVertex.texture.x = textureCoords[tindex-1].x;
							newVertex.texture.y = 1-textureCoords[tindex-1].y;
							newVertex.normal = normals[nindex-1];

							data->vertices.push_back(newVertex);

							createdVertices[vertNumber] = data->vertices.size() - 1;
						}
						
						data->indices.push_back(createdVertices[vertNumber]);
						++index;
					}
				
				}
			}
		}

		createdVertices.clear();

		this->m_ModelData[filename] = data;
	}

	return this->m_ModelData[filename];
}