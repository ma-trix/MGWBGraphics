#pragma once
#include <string>
#include <GL/glew.h>
#include "GLTexture.h"

namespace MatrixEngine {
	struct Color;
	struct Vertex3D;

	class Voxel
	{
	public:
		Voxel();
		~Voxel();

		void draw();
		void init(float x, float y, float z, float width, float height, float depth, std::string texturePath);
		void init(float x, float y, float z, float width, float height, float depth, const std::string (&texturePaths)[6]);
		void setAllVertexData(float x, float y, float z, float width, float height, float depth);
	private:
		void loadFaceTextures(const std::string(&texturePaths)[6]);
		void Voxel::setVertexDataFace1Top(float x, float y, float z, float width, float height, float depth, Color color, Vertex3D vertexData[36]);
		void Voxel::setVertexDataFace2Front(float x, float y, float z, float width, float height, Color color, Vertex3D vertexData[36]);
		void Voxel::setVertexDataFace3Right(float x, float y, float z, float width, float height, float depth, Color color, Vertex3D vertexData[36]);
		void Voxel::setVertexDataFace4Back(float x, float y, float z, float width, float height, float depth, Color color, Vertex3D vertexData[36]);
		void Voxel::setVertexDataFace5Left(float x, float y, float z, float height, float depth, Color color, Vertex3D vertexData[36]);
		void Voxel::setVertexDataFace6Bottom(float x, float y, float z, float width, float depth, Color color, Vertex3D vertexData[36]);
		void generateBufferIfDoesntExist(GLuint &buffer);
		GLuint _vao;
		GLuint _vboID;
		GLTexture _face[6];
	};
}