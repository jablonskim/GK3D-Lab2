#pragma once

#include "Includes.h"
#include "Vertex.h"
#include "Settings.h"

class Mesh
{
public:
	~Mesh();
	static std::vector<std::shared_ptr<Mesh>> createTerrain();
	static std::vector<std::shared_ptr<Mesh>> createCube();
	static std::vector<std::shared_ptr<Mesh>> createSphere();
	static std::vector<std::shared_ptr<Mesh>> createPostprocessingQuad();
	static std::vector<std::shared_ptr<Mesh>> fromFile(std::string filename);
	void draw();

private:
	Mesh();
	void setupArrays();
	static void processNode(aiNode *node, const aiScene* scene, std::vector<std::shared_ptr<Mesh>> &meshes);
	static std::shared_ptr<Mesh> processLoadedMesh(aiMesh *loaded_mesh);
	void calculateNormals();

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	Mesh(const Mesh &) = delete;
	Mesh& operator=(const Mesh &) = delete;

};

