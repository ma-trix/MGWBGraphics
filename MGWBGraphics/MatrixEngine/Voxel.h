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

	private:
		float _x;
		float _y;
		float _z;
		float _width;
		float _height;
		float _depth;
		GLuint _vboID;
		GLTexture _face[6];
		GLuint _vao;
	};
}