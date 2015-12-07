#include "Mesh.h"

Mesh::Mesh() :
	vao(0),
	vbo(0),
	ebo(0)
{
	std::cout << "Creating new mesh..." << std::endl;
}

void Mesh::setupArrays()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, position)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, texture_coords)));

	glBindVertexArray(0);
}

void Mesh::processNode(aiNode * node, const aiScene * scene, std::vector<std::shared_ptr<Mesh>> &meshes)
{
	std::cout << "Processing scene node..." << std::endl;

	for (GLuint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* m = scene->mMeshes[node->mMeshes[i]];
		auto mesh = processLoadedMesh(m);
		meshes.push_back(mesh);
	}

	for (GLuint i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene, meshes);
	}
}

std::shared_ptr<Mesh> Mesh::processLoadedMesh(aiMesh * loaded_mesh)
{
	std::cout << "Processing mesh..." << std::endl;

	auto mesh = std::shared_ptr<Mesh>(new Mesh());

	for (GLuint i = 0; i < loaded_mesh->mNumVertices; ++i)
	{
		Vertex v = {
			glm::vec3(loaded_mesh->mVertices[i].x, loaded_mesh->mVertices[i].y, loaded_mesh->mVertices[i].z),
			glm::vec3(loaded_mesh->mNormals[i].x, loaded_mesh->mNormals[i].y, loaded_mesh->mNormals[i].z)
		};

		mesh->vertices.push_back(v);
	}

	for (GLuint i = 0; i < loaded_mesh->mNumFaces; ++i)
	{
		aiFace face = loaded_mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; ++j)
		{
			mesh->indices.push_back(face.mIndices[j]);
		}
	}

	return mesh;
}

void Mesh::calculateNormals()
{
	for (auto i = std::begin(indices); i != std::end(indices); std::advance(i, 3))
	{
		glm::vec3 v[] = { vertices[i[0]].position, vertices[i[1]].position, vertices[i[2]].position };
		glm::vec3 norm = glm::cross(v[1] - v[0], v[2] - v[0]);

		for (int j = 0; j < 3; ++j)
		{
			vertices[*(i + j)].normal += norm;
		}
	}

	for (auto i = std::begin(vertices); i != std::end(vertices); std::advance(i, 1))
		i->normal = glm::normalize(i->normal);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vbo);
	glDeleteVertexArrays(1, &ebo);

	std::cout << "Destroying mesh..." << std::endl;
}

std::vector<std::shared_ptr<Mesh>> Mesh::createTerrain()
{
	auto m = std::shared_ptr<Mesh>(new Mesh());

	int num_vertices = Settings::TerrainSize;

	float ter_from = -1.f;
	float ter_to = 1.f;
	float ter_s = ter_to - ter_from;
	noise::module::Perlin perlin;

	for (int i = 0; i <= num_vertices; ++i)
	{
		for (int j = 0; j <= num_vertices; ++j)
		{
			float x = (i * ter_s / num_vertices) + ter_from;
			float y = (j * ter_s / num_vertices) + ter_from;

			Vertex v;
			float z = 0.05f * static_cast<float>(perlin.GetValue(x + 1.0, y + 1.0, 0.5));
			v.position = glm::vec3(x, y, z);
			v.normal = glm::vec3(0.f, 0.f, 0.f);
			//v.texture_coords = glm::vec2(j & 1 ? 1.f : 2.f, i & 1 ? 2.f : 1.f);
			v.texture_coords = glm::vec2(j, i);

			m->vertices.push_back(v);
		}
	}

	int n = Settings::TerrainSize;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			m->indices.push_back((i + 0) * (n + 1) + j + 0);
			m->indices.push_back((i + 1) * (n + 1) + j + 0);
			m->indices.push_back((i + 0) * (n + 1) + j + 1);

			m->indices.push_back((i + 0) * (n + 1) + j + 1);
			m->indices.push_back((i + 1) * (n + 1) + j + 0);
			m->indices.push_back((i + 1) * (n + 1) + j + 1);
		}
	}

	m->calculateNormals();

	// Nowa tekstura
	for (int i = 0; i < 500; ++i)
	{
		int x = rand() % n;
		int y = rand() % n;

		int id = (x * n + y) * 6;
		int new_base = m->vertices.size();

		auto v1 = m->vertices[m->indices[id + 0]];
		auto v2 = m->vertices[m->indices[id + 2]];
		auto v3 = m->vertices[m->indices[id + 1]];
		auto v4 = m->vertices[m->indices[id + 5]];

		auto change_coords = [](auto &v) {
			v.texture_coords.x = static_cast<int>(v.texture_coords.x) & 1 ? 1.f : 0.f;
			v.texture_coords.y = static_cast<int>(v.texture_coords.y) & 1 ? 1.f : 0.f;
		};

		change_coords(v1);
		change_coords(v2);
		change_coords(v3);
		change_coords(v4);

		m->vertices.push_back(v1);
		m->vertices.push_back(v2);
		m->vertices.push_back(v3);
		m->vertices.push_back(v4);

		m->indices[id + 0] = new_base + 0;
		m->indices[id + 1] = new_base + 2;
		m->indices[id + 2] = new_base + 1;
		m->indices[id + 3] = new_base + 1;
		m->indices[id + 4] = new_base + 2;
		m->indices[id + 5] = new_base + 3;
	}

	m->setupArrays();

	return { m };
}

std::vector<std::shared_ptr<Mesh>> Mesh::createCube()
{
	auto m = std::shared_ptr<Mesh>(new Mesh());

	glm::vec3 norm_z_p(0.f, 0.f, 1.f);
	glm::vec3 norm_z_m(0.f, 0.f, -1.f);
	glm::vec3 norm_x_p(1.f, 0.f, 0.f);
	glm::vec3 norm_x_m(-1.f, 0.f, 0.f);
	glm::vec3 norm_y_p(0.f, 1.f, 0.f);
	glm::vec3 norm_y_m(0.f, -1.f, 0.f);

	glm::vec3 v_ppp(1.f, 1.f, 1.f);
	glm::vec3 v_ppm(1.f, 1.f, -1.f);
	glm::vec3 v_pmp(1.f, -1.f, 1.f);
	glm::vec3 v_pmm(1.f, -1.f, -1.f);
	glm::vec3 v_mpp(-1.f, 1.f, 1.f);
	glm::vec3 v_mpm(-1.f, 1.f, -1.f);
	glm::vec3 v_mmp(-1.f, -1.f, 1.f);
	glm::vec3 v_mmm(-1.f, -1.f, -1.f);

	glm::vec2 t_00(0.f, 0.f);
	glm::vec2 t_01(0.f, 1.f);
	glm::vec2 t_10(1.f, 0.f);
	glm::vec2 t_11(1.f, 1.f);

	// Front
	m->vertices.push_back({ v_mmp, norm_z_p, t_00 });
	m->vertices.push_back({ v_pmp, norm_z_p, t_10 });
	m->vertices.push_back({ v_ppp, norm_z_p, t_11 });

	m->vertices.push_back({ v_ppp, norm_z_p, t_11 });
	m->vertices.push_back({ v_mpp, norm_z_p, t_01 });
	m->vertices.push_back({ v_mmp, norm_z_p, t_00 });

	// Back
	m->vertices.push_back({ v_mmm, norm_z_m, t_00 });
	m->vertices.push_back({ v_ppm, norm_z_m, t_11 });
	m->vertices.push_back({ v_pmm, norm_z_m, t_10 });

	m->vertices.push_back({ v_ppm, norm_z_m, t_11 });
	m->vertices.push_back({ v_mmm, norm_z_m, t_00 });
	m->vertices.push_back({ v_mpm, norm_z_m, t_01 });

	// Left
	m->vertices.push_back({ v_mpp, norm_x_m, t_10 });
	m->vertices.push_back({ v_mpm, norm_x_m, t_11 });
	m->vertices.push_back({ v_mmm, norm_x_m, t_01 });

	m->vertices.push_back({ v_mmm, norm_x_m, t_01 });
	m->vertices.push_back({ v_mmp, norm_x_m, t_00 });
	m->vertices.push_back({ v_mpp, norm_x_m, t_10 });

	// Right
	m->vertices.push_back({ v_ppp, norm_x_p, t_10 });
	m->vertices.push_back({ v_pmm, norm_x_p, t_01 });
	m->vertices.push_back({ v_ppm, norm_x_p, t_11 });

	m->vertices.push_back({ v_pmm, norm_x_p, t_01 });
	m->vertices.push_back({ v_ppp, norm_x_p, t_10 });
	m->vertices.push_back({ v_pmp, norm_x_p, t_00 });

	// Top
	m->vertices.push_back({ v_mpm, norm_y_p, t_01 });
	m->vertices.push_back({ v_ppp, norm_y_p, t_10 });
	m->vertices.push_back({ v_ppm, norm_y_p, t_11 });

	m->vertices.push_back({ v_ppp, norm_y_p, t_10 });
	m->vertices.push_back({ v_mpm, norm_y_p, t_01 });
	m->vertices.push_back({ v_mpp, norm_y_p, t_00 });

	// Bottom
	m->vertices.push_back({ v_mmm, norm_y_m, t_01 });
	m->vertices.push_back({ v_pmm, norm_y_m, t_11 });
	m->vertices.push_back({ v_pmp, norm_y_m, t_10 });

	m->vertices.push_back({ v_pmp, norm_y_m, t_10 });
	m->vertices.push_back({ v_mmp, norm_y_m, t_00 });
	m->vertices.push_back({ v_mmm, norm_y_m, t_01 });

	for (size_t i = 0; i < m->vertices.size(); ++i)
	{
		m->indices.push_back(i);
	}

	m->setupArrays();

	return { m };
}

std::vector<std::shared_ptr<Mesh>> Mesh::createSphere()
{
	auto m = std::shared_ptr<Mesh>(new Mesh());

	int stacks = 100;
	int slices = 100;
	float radius = 1.0;

	for (int i = 0; i <= stacks; ++i) {

		float V = i / (float)stacks;
		float phi = V * glm::pi <float>();

		for (int j = 0; j <= slices; ++j) {

			float U = j / (float)slices;
			float theta = U * (glm::pi <float>() * 2);

			float x = cosf(theta) * sinf(phi);
			float y = cosf(phi);
			float z = sinf(theta) * sinf(phi);

			m->vertices.push_back({ glm::vec3(x, y, z) * radius });
		}
	}

	for (int i = 0; i < slices * stacks + slices; ++i) {

		m->indices.push_back(i);
		m->indices.push_back(i + slices + 1);
		m->indices.push_back(i + slices);

		m->indices.push_back(i + slices + 1);
		m->indices.push_back(i);
		m->indices.push_back(i + 1);
	}

	m->calculateNormals();
	m->setupArrays();

	return { m };
}

std::vector<std::shared_ptr<Mesh>> Mesh::createPostprocessingQuad()
{
	auto m = std::shared_ptr<Mesh>(new Mesh());

	m->vertices.push_back({ glm::vec3(-1.f, 1.f, 0.f), glm::vec3(), glm::vec2(0.f, 1.f) });
	m->vertices.push_back({ glm::vec3(-1.f, -1.f, 0.f), glm::vec3(), glm::vec2(0.f, 0.f) });
	m->vertices.push_back({ glm::vec3(1.f, -1.f, 0.f), glm::vec3(), glm::vec2(1.f, 0.f) });

	m->vertices.push_back({ glm::vec3(-1.f, 1.f, 0.f), glm::vec3(), glm::vec2(0.f, 1.f) });
	m->vertices.push_back({ glm::vec3(1.f, -1.f, 0.f), glm::vec3(), glm::vec2(1.f, 0.f) });
	m->vertices.push_back({ glm::vec3(1.f, 1.f, 0.f), glm::vec3(), glm::vec2(1.f, 1.f) });

	for (size_t i = 0; i < m->vertices.size(); ++i)
	{
		m->indices.push_back(i);
	}

	m->setupArrays();

	return { m };
}

std::vector<std::shared_ptr<Mesh>> Mesh::fromFile(std::string filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Error while loading model: " << importer.GetErrorString() << std::endl;
		return std::vector<std::shared_ptr<Mesh>>();
	}

	std::vector<std::shared_ptr<Mesh>> meshes;

	processNode(scene->mRootNode, scene, meshes);
	std::for_each(std::begin(meshes), std::end(meshes), [](auto mesh) { mesh->setupArrays(); });

	return meshes;
}

void Mesh::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
