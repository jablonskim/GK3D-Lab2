#include "Cubemap.h"

bool Cubemap::loadFromFiles(const char * files_base_name)
{
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	for (int i = 0; i < 6; ++i)
	{
		int width, height;
		std::string filename = std::string(files_base_name) + std::to_string(i) + std::string(Settings::SkyboxExtension);
		std::cerr << filename << std::endl;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		std::cerr << width << " " << height << " " << (int)image << std::endl;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

Cubemap::Cubemap()
{
	program = ShaderProgram::create(Settings::VertexShaderSkyboxPath, Settings::FragmentShaderSkyboxPath);
	cube = Model::createSkybox(program);
}

std::shared_ptr<Cubemap> Cubemap::fromFiles(const char * files_base_name)
{
	auto cubemap = std::shared_ptr<Cubemap>(new Cubemap());

	if (!cubemap->loadFromFiles(files_base_name))
		return std::shared_ptr<Cubemap>();

	return cubemap;
}

Cubemap::~Cubemap()
{
}

void Cubemap::render(std::function<glm::mat4()> projection_getter, std::function<glm::mat4()> view_getter)
{
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);

	program->use(false);

	auto view = glm::mat4(glm::mat3(view_getter()));
	glUniformMatrix4fv(program->getUniformLocation(Settings::ShaderViewMatrixLocationName), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(program->getUniformLocation(Settings::ShaderProjectionMatrixLocationName), 1, GL_FALSE, glm::value_ptr(projection_getter()));

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(program->getUniformLocation(Settings::ShaderSkyboxLocationName), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	cube->draw();

	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
}
