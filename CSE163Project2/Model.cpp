#include "Model.h"
#include <stdio.h>
#include <iostream>
#include <Windows.h>

void Model::Draw(Shader shader,mat4 modelview)
{
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		Mesh flatMesh = meshes[i].createFlatMesh();
		//this -> meshes[i].Draw(shader,modelview);
		flatMesh.Draw(shader, modelview);
	}
}

void Model::loadModel(string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}

}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// Process vertex positions, normals and texture coordinates
		glm::vec3 vector;

		//vertex
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		////normal 
		vec3 normal = glm::normalize(vector);
		//vector.x = mesh->mNormals[i].x;
		//vector.y = mesh->mNormals[i].y;
		//vector.z = mesh->mNormals[i].z;
		vertex.Normal = normal;
		vertices.push_back(vertex);

		//faces

		//texture
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
	}
	// Process indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	Material mtl;

	// Process material
	if (mesh->mMaterialIndex >= 0)
	{
		if (mesh->mMaterialIndex >= 0)
		{

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			material->Get(AI_MATKEY_COLOR_AMBIENT, mtl.ambient);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, mtl.diffuse);
			material->Get(AI_MATKEY_COLOR_SPECULAR, mtl.specular);
			material->Get(AI_MATKEY_SHININESS, mtl.shininess);
		}
	}
	char buff[100];
	sprintf_s(buff, "Number of vertices:%d\n",vertices.size());
	OutputDebugString(buff);

	return Mesh(vertices, indices, textures, mtl);
}


