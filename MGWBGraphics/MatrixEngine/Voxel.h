#pragma once
#include <string>
#include <GL/glew.h>
#include "GLTexture.h"

namespace MatrixEngine {
	class Voxel
	{
	public:
		Voxel();
		~Voxel();

		void draw();
		void init(float x, float y, float z, float width, float height, float depth, std::string texturePath);
		void setAllVertexData(float x, float y, float z, float width, float height, float depth);
		void init(float x, float y, float z, float width, float height, float depth, const std::string (&texturePaths)[6]);
	private:
		void loadFaceTextures(const std::string(&texturePaths)[6]);
		void vboCheck();
		GLuint _vao;
		GLuint _vboID;
		GLTexture _face[6];
	};
}