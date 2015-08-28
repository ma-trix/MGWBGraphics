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
		void init(float x, float y, float z, float width, float height, float depth, std::string texturePath1, std::string texturePath2, std::string texturePath3, std::string texturePath4, std::string texturePath5, std::string texturePath6);
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