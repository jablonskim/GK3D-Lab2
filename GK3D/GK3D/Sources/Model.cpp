#include "Model.h"

Model::Model(std::shared_ptr<ShaderProgram> prog, std::vector<std::shared_ptr<Mesh>> meshes) :
	program(prog),
	model_meshes(meshes)
{
	color = glm::vec4(0.f, 0.f, 0.f, 0.f);
	setMatrix(glm::mat4());

	std::cout << "Creating model..." << std::endl;
}

void Model::useColor()
{
	glUniform4fv(program->getUniformLocation(Settings::ShaderModelColorLocationName), 1, glm::value_ptr(color));
}

void Model::useMatrix()
{
	GLint model_mat = program->getUniformLocation(Settings::ShaderModelMatrixLocationName);
	glUniformMatrix4fv(model_mat, 1, GL_FALSE, glm::value_ptr(model_matrix));

	GLint normal_mat = program->getUniformLocation(Settings::ShaderNormalMatrixLocationName);
	glUniformMatrix3fv(normal_mat, 1, GL_FALSE, glm::value_ptr(normal_matrix));
}

std::shared_ptr<Model> Model::fromMeshes(std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<ShaderProgram> prog)
{
	return std::shared_ptr<Model>(new Model(prog, meshes));
}

std::shared_ptr<Model> Model::createTerrain(std::shared_ptr<ShaderProgram> prog)
{
	auto m = Model::fromMeshes(Mesh::createTerrain(), prog);

	//m->setColor(glm::vec4(0.f, 0.392157f, 0.f, 1.f));
	m->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));
	auto rotated = glm::rotate(m->model_matrix, glm::radians(90.f), glm::vec3(-1.f, 0.f, 0.f));
	auto scaled = glm::scale(rotated, glm::vec3(Settings::TerrainScaleFactor));
	m->setMatrix(scaled);

	return m;
}

std::shared_ptr<Model> Model::createPostprocessingQuad(std::shared_ptr<ShaderProgram> prog)
{
	auto m = Model::fromMeshes(Mesh::createPostprocessingQuad(), prog);
	m->setColor(glm::vec4(1.f, 1.f, 1.f, 1.f));

	return m;
}

Model::~Model()
{
	std::cout << "Destroying model..." << std::endl;
}

void Model::draw()
{
	useColor();
	useMatrix();

	for(auto & m : model_meshes)
		m->draw();
}

void Model::setColor(glm::vec4 col)
{
	color = col;
}

void Model::setMatrix(glm::mat4 matrix)
{
	model_matrix = matrix;
	normal_matrix = glm::mat3(glm::transpose(glm::inverse(matrix)));
}

void Model::setMatrixFromDefaults(glm::mat4 base, glm::vec3 translation, GLfloat scale, GLfloat angle, glm::vec3 axis)
{
	auto translated = glm::translate(base, translation);
	auto scaled = glm::scale(translated, glm::vec3(scale));
	auto rotated = glm::rotate(scaled, glm::radians(angle), axis);

	setMatrix(rotated);
}
