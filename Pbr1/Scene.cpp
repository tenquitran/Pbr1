#include "stdafx.h"
#include "Scene.h"

#pragma warning(push) 
#pragma warning(disable : 4100)
// stb_image loader.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop)

//////////////////////////////////////////////////////////////////////////

using namespace CommonLibOgl;
using namespace Pbr1App;

//////////////////////////////////////////////////////////////////////////


Scene::Scene()
	: m_vao{}, m_vbo{}, m_index{}, m_indexCount{}, m_normals{}, m_texCoord{}, 
	  m_texAlbedo{}, m_texNormal{}, m_texMetallic{}, m_texRoughness{}, m_texAo{}
{
}

Scene::~Scene()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (0 != m_texAo)
		{glDeleteTextures(1, &m_texAo);}

	if (0 != m_texRoughness)
		{glDeleteTextures(1, &m_texRoughness);}

	if (0 != m_texMetallic)
		{glDeleteTextures(1, &m_texMetallic);}

	if (0 != m_texNormal)
		{glDeleteTextures(1, &m_texNormal);}

	if (0 != m_texAlbedo)
		{glDeleteTextures(1, &m_texAlbedo);}

	if (0 != m_normals)
		{glDeleteBuffers(1, &m_normals);}

	if (0 != m_texCoord)
		{glDeleteBuffers(1, &m_texCoord);}

	if (0 != m_index)
		{glDeleteBuffers(1, &m_index);}

	if (0 != m_vbo)
		{glDeleteBuffers(1, &m_vbo);}

	if (0 != m_vao)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);
	}
}

bool Scene::initialize(GLfloat aspectRatio, const OpenGLInfo& openGlInfo)
{
	// Initial scale factor for the camera.
	const GLfloat CameraScaleFactor = 1.0f;

	m_spCamera = std::make_unique<Camera>(aspectRatio, CameraScaleFactor,
		openGlInfo.FieldOfView, openGlInfo.FrustumNear, openGlInfo.FrustumFar);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0.8f, 0.93f, 0.96f, 1.0f);    // very light blue
	//glClearColor(0.0f, 0.64f, 0.91f, 1.0f);    // light blue

	// Initialize the program wrapper.

	const ShaderCollection shaders = {
		{ GL_VERTEX_SHADER,   "shaders\\pbr.vert" },
		{ GL_FRAGMENT_SHADER, "shaders\\pbr.frag" }
	};

	m_spProgram = std::make_unique<ProgramGLSL>(shaders);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// A cube.

	float side = 1.0f;
	float side2 = side / 2.0f;

	GLfloat vertices[] = {
		// Front
		-side2, -side2, side2,
		side2, -side2, side2,
		side2, side2, side2,
		-side2, side2, side2,
		// Right
		side2, -side2, side2,
		side2, -side2, -side2,
		side2, side2, -side2,
		side2, side2, side2,
		// Back
		-side2, -side2, -side2,
		-side2, side2, -side2,
		side2, side2, -side2,
		side2, -side2, -side2,
		// Left
		-side2, -side2, side2,
		-side2, side2, side2,
		-side2, side2, -side2,
		-side2, -side2, -side2,
		// Bottom
		-side2, -side2, side2,
		-side2, -side2, -side2,
		side2, -side2, -side2,
		side2, -side2, side2,
		// Top
		-side2, side2, side2,
		side2, side2, side2,
		side2, side2, -side2,
		-side2, side2, -side2
	};

	// Generate VBO and fill it with the data.

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, _countof(vertices) * sizeof(vertices[0]), vertices, GL_STATIC_DRAW);

	// Fill in the vertex position attribute.
	const GLuint attrVertexPos = 0;
	glVertexAttribPointer(attrVertexPos, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(attrVertexPos);

	// Fill the index buffer.

	GLuint indices[] = {
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	m_indexCount = _countof(indices);

	glGenBuffers(1, &m_index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _countof(indices) * sizeof(indices[0]), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLfloat normals[] = {
		// Front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		// Right
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		// Back
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		// Left
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		// Bottom
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		// Top
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenBuffers(1, &m_normals);
	glBindBuffer(GL_ARRAY_BUFFER, m_normals);
	glBufferData(GL_ARRAY_BUFFER, _countof(normals) * sizeof(normals[0]), normals, GL_STATIC_DRAW);

	// Fill in the normal attribute.
	const GLuint attrNormal = 1;
	glVertexAttribPointer(attrNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(attrNormal);

	GLfloat texCoords[] = {
		// Front
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Right
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Back
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Left
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Bottom
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// Top
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	glGenBuffers(1, &m_texCoord);
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoord);
	glBufferData(GL_ARRAY_BUFFER, _countof(texCoords) * sizeof(texCoords[0]), texCoords, GL_STATIC_DRAW);

	// Fill in the texture coordinate attribute.
	const GLuint attrTexCoord = 2;
	glVertexAttribPointer(attrTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(attrTexCoord);

	if (!setTexturesAndLights())
	{
		std::wcerr << L"Failed to set textures and/or lights\n";
		assert(false); return false;
	}

	if (!m_spProgram->validate())
	{
		std::wcerr << L"GLSL program validation failed\n";
		assert(false); return false;
	}

	return true;
}

bool Scene::setTexturesAndLights()
{
	// Create and load textures.

	GLuint textures[] = { m_texAlbedo, m_texNormal, m_texMetallic, m_texRoughness, m_texAo };

#if 0
	std::string filePaths[] = {
		"data/TitaniumScuffed/Titanium-Scuffed_basecolor.png",
		"data/TitaniumScuffed/Titanium-Scuffed_normal.png",
		"data/TitaniumScuffed/Titanium-Scuffed_metallic.png",
		"data/TitaniumScuffed/Titanium-Scuffed_roughness.png",
		"data/TitaniumScuffed/Titanium-Scuffed_ao.png", };
#else
	std::string filePaths[] = {
		"data/parquet/parquet_albedo512.png",
		"data/parquet/parquet_normal.png",
		"data/parquet/parquet_metallic.png",
		"data/parquet/parquet_roughness.png",
		"data/parquet/parquet_AO.png", };
#endif

	auto del = [](unsigned char* pBuff) {
		stbi_image_free(pBuff);
	};

	const size_t FileCount = _countof(filePaths);

	for (size_t i = 0; i < FileCount; ++i)
	{
		int width = {}, height = {};
		int components = {};    // number of components per pixel

		std::unique_ptr<unsigned char[], decltype(del)> spData(stbi_load(filePaths[i].c_str(), &width, &height, &components, 0));
		if (!spData)
		{
			std::wcerr << L"Failed to load texture " << filePaths[i].c_str() << '\n';
			assert(false); return false;
		}

		GLenum format = {};

		switch (components)
		{
		case 1:
			format = GL_RED; break;
		case 3:
			format = GL_RGB; break;
		case 4:
			format = GL_RGBA; break;
		default:
			std::wcerr << L"Unexpected number of components per pixel: " << filePaths[i].c_str() << '\n';
			assert(false); break;
		}

		glActiveTexture(GL_TEXTURE0 + i);

		glGenTextures(1, &textures[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, spData.get());

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glUseProgram(m_spProgram->getProgram());

	// Set texture samplers.
	glUniform1i(11, 0);
	glUniform1i(12, 1);
	glUniform1i(13, 2);
	glUniform1i(14, 3);
	glUniform1i(15, 4);

	// Set light position uniforms.

	glm::vec3 lightPositions[] = {
		{  0.0f,  2.0f, 0.0f },
		{  0.0f, -2.0f, 0.0f },
		{  2.0f,  0.0f, 0.7f },
		{ -2.0f,  0.0f, 0.0f } };

	glUniform3fv(2, 1, glm::value_ptr(lightPositions[0]));
	glUniform3fv(3, 1, glm::value_ptr(lightPositions[1]));
	glUniform3fv(4, 1, glm::value_ptr(lightPositions[2]));
	glUniform3fv(5, 1, glm::value_ptr(lightPositions[3]));

	// Set light color uniforms.

	glm::vec3 lightColors[] = {
		{ 50.0f, 50.0f, 150.0f },
		{ 150.0f, 150.0f, 150.0f },
		{ 150.0f, 150.0f, 150.0f },
		{ 150.0f, 150.0f, 150.0f } };

	glUniform3fv(6, 1, glm::value_ptr(lightColors[0]));
	glUniform3fv(7, 1, glm::value_ptr(lightColors[1]));
	glUniform3fv(8, 1, glm::value_ptr(lightColors[2]));
	glUniform3fv(9, 1, glm::value_ptr(lightColors[3]));

	glUseProgram(0);

	return true;
}

void Scene::updateViewMatrices() const
{
	assert(m_spProgram);

	glUseProgram(m_spProgram->getProgram());

	glm::mat4 model = m_spCamera->getModelMatrix();

	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 mvp = m_spCamera->getModelViewProjectionMatrix();

	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(mvp));

#if 0
	glm::mat4 mv = m_spCamera->getModelViewMatrix();

	glm::mat3 normal = glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]));
	glUniformMatrix3fv(m_unNormal, 1, GL_FALSE, glm::value_ptr(normal));
#endif

	// Set the camera position uniform.
#if 0
	// TODO: causes strange shadows on the scene object.
	glUniform3fv(10, 1, glm::value_ptr(m_spCamera->getTranslation()));
#else
	// TODO: temp
	glm::vec3 cameraPos(0.0f, 0.0f, -5.0f);
	glUniform3fv(10, 1, glm::value_ptr(cameraPos));
#endif

	glUseProgram(0);
}

void Scene::translateCameraX(GLfloat diff)
{
	m_spCamera->translateX(diff);
}

void Scene::translateCameraY(GLfloat diff)
{
	m_spCamera->translateY(diff);
}

void Scene::translateCameraZ(GLfloat diff)
{
	m_spCamera->translateZ(diff);
}

void Scene::rotateCameraX(GLfloat angleDegrees)
{
	m_spCamera->rotateX(angleDegrees);
}

void Scene::rotateCameraY(GLfloat angleDegrees)
{
	m_spCamera->rotateY(angleDegrees);
}

void Scene::rotateCameraZ(GLfloat angleDegrees)
{
	m_spCamera->rotateZ(angleDegrees);
}

void Scene::rotateCameraXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees)
{
	m_spCamera->rotateXY(xAngleDegrees, yAngleDegrees);
}

GLfloat Scene::getCameraScale() const
{
	return m_spCamera->getScale();
}

void Scene::scaleCamera(GLfloat amount)
{
	m_spCamera->scale(amount);
}

void Scene::resize(GLfloat aspectRatio)
{
	m_spCamera->resize(aspectRatio);
}

void Scene::render() const
{
	updateViewMatrices();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	assert(m_spProgram);

	glUseProgram(m_spProgram->getProgram());
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
