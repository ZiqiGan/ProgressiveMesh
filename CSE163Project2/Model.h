#pragma once
#include "Mesh.h"
class Model
{
public:
	/*  Functions   */
	Model(){}
	Model(GLchar* path)
	{
		this->loadModel(path);
	}
	void Draw(Shader shader,mat4 modelview);
private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	/*  Functions   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};
