#pragma once

#include "Includes.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

class Model
{
public:
	static std::shared_ptr<Model> fromMeshes(std::vector<std::shared_ptr<Mesh>> mesh, std::shared_ptr<ShaderProgram> prog);
	static std::shared_ptr<Model> createTerrain(std::shared_ptr<ShaderProgram> prog);
	static std::shared_ptr<Model> createCube(std::shared_ptr<ShaderProgram> prog);
	static std::shared_ptr<Model> createPostprocessingQuad(std::shared_ptr<ShaderProgram> prog);

	~Model();
	void draw();
	void setColor(glm::vec4 col);
	void setMatrix(glm::mat4 matrix);
	void setMatrixFromDefaults(glm::mat4 base, glm::vec3 translation, GLfloat scale, GLfloat angle, glm::vec3 axis);
	void addTexture(std::shared_ptr<Texture> texture);

private:
	Model(std::shared_ptr<ShaderProgram> prog, std::vector<std::shared_ptr<Mesh>> meshes);

	void useColor();
	void useMatrix();
	void useTextures();

	std::shared_ptr<ShaderProgram> program;
	std::vector<std::shared_ptr<Mesh>> model_meshes;
	std::vector<std::shared_ptr<Texture>> textures;
	
	glm::mat4 model_matrix;
	glm::mat3 normal_matrix;
	glm::vec4 color;
};

