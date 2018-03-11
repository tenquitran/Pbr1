#pragma once


namespace Pbr1App
{
	// OpenGL scene.
	class Scene
	{
	public:
		Scene();

		virtual ~Scene();

		// Parameters: aspectRatio - aspect ratio of the window's client area;
		//             openGlInfo - OpenGL settings.
		bool initialize(GLfloat aspectRatio, const CommonLibOgl::OpenGLInfo& openGlInfo);

		// Update Model-View-Projection (MVP) and other matrices in the GLSL program.
		void updateViewMatrices() const;

		void resize(GLfloat aspectRatio);

		void render() const;

		//////////////////////////////////////////////////////////////////////////
		// Camera control.
		//////////////////////////////////////////////////////////////////////////

		void translateCameraX(GLfloat diff);
		void translateCameraY(GLfloat diff);
		void translateCameraZ(GLfloat diff);

		void rotateCameraX(GLfloat angleDegrees);
		void rotateCameraY(GLfloat angleDegrees);
		void rotateCameraZ(GLfloat angleDegrees);

		void rotateCameraXY(GLfloat xAngleDegrees, GLfloat yAngleDegrees);

		GLfloat getCameraScale() const;

		void scaleCamera(GLfloat amount);

	private:
		bool setTexturesAndLights();

	private:
		std::unique_ptr<CommonLibOgl::ProgramGLSL> m_spProgram;

		// OpenGL camera.
		std::unique_ptr<CommonLibOgl::Camera> m_spCamera;

		GLuint m_vao;
		GLuint m_vbo;

		GLuint m_index;          // index buffer
		GLsizei m_indexCount;    // number of indices

		GLuint m_normals;

		GLuint m_texCoord;

		// Textures.
		GLuint m_texAlbedo;
		GLuint m_texNormal;
		GLuint m_texMetallic;
		GLuint m_texRoughness;
		GLuint m_texAo;
	};
}
