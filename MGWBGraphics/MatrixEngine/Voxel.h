#pragma once
#include <string>
#include <GL/glew.h>
#include "GLTexture.h"
#include "Vertex.h"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace MatrixEngine {
	struct Face
	{
		Face(){}
		Face(int ID, int v1, int v2, int v3, int v4)
		{
			id = ID;
			vertices = glm::ivec4{v1, v2, v3, v4};
		}
		int id;
		glm::ivec4 vertices;
	};

	class Voxel
	{
	public:
		Voxel();
		~Voxel();

		void draw();
		void rotate(float angle, glm::vec3 axisInObjectCoord);
		void Voxel::translate(glm::vec3 v);
		void init(float x, float y, float z, float width, float height, float depth, std::string texturePath);
		void updateVertexPositions();
		void updateFaceSetup();
		void updateSpatialDiagonalPosition();
		void init(float x, float y, float z, float width, float height, float depth, const std::string (&texturePaths)[6]);
		void bufferVertexData();
		void setAllVertexData();
		void setVertexDataForFace(int face, Color color);
		void resetPosition();
		std::string printPosition();
		glm::mat4 getObject2world() { return _object2world; };

	private:
		void loadFaceTextures(const std::string(&texturePaths)[6]);
		GLuint _vao;
		GLuint _vboID;
		GLTexture _face[6];
		Vertex3D _vertexData[36];
		Position3D _lbf;		// left-bottom-front vertex position (vertex E) in world coordinates
		Position3D _dimensions;	// width(X), height(Y), depth(Z)
		Position3D _rtb;		// right-top-back vertex position (vertex A) in world coordinates
		Position3D _vertices[8];
		const static int _FACECOUNT = 6;
		Face _faces[_FACECOUNT];
		UV _TOPRIGHT = { 1.0f, 1.0f };
		UV _TOPLEFT = { 0.0f, 1.0f };
		UV _BOTRIGHT = { 1.0f, 0.0f };
		UV _BOTLEFT = { 0.0f, 0.0f };
		glm::mat4 _object2world;
		glm::vec4 _spatialDiagonal;
	};
}