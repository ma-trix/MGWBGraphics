#include "Voxel.h"
#include "ResourceManager.h"
#include "Vertex.h"


namespace MatrixEngine {
	Voxel::Voxel()
	{
		_vao = 0;
		_vboID = 0;
	}

	Voxel::~Voxel()
	{
		if (_vboID != 0)
		{
			glDeleteBuffers(1, &_vboID);
		}
		if (_vao != 0)
		{
			glDeleteBuffers(1, &_vao);
		}
	}

	void Voxel::draw()
	{
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);
		
		glBindVertexBuffer(0, _vboID, offsetof(Vertex3D, position), sizeof(Vertex3D));

		glEnableVertexAttribArray(0);
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, position));
		glVertexAttribBinding(0, 0);
		glEnableVertexAttribArray(1);
		glVertexAttribFormat(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex3D, color));
		glVertexAttribBinding(1, 0);
		glEnableVertexAttribArray(2);
		glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, uv));
		glVertexAttribBinding(2, 0);

		for (int i = 0; i < 6; i++) 
		{
			glBindTexture(GL_TEXTURE_2D, _face[i].id);
			glDrawArrays(GL_TRIANGLES, i*6, 6);
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	void setVertexData(Vertex3D &vertex, float x, float y, float z, float u, float v, GLuint r, GLuint g, GLuint b, GLuint a)
	{
		vertex.setPosition(x, y, z);
		vertex.setUV(u, v);
		vertex.setColor(r, g, b, a);
	}

	void Voxel::init(float x, float y, float z, float width, float height, float depth, const std::string singleTexturePath)
	{
		init(x, y, z, width, height, depth, { singleTexturePath, singleTexturePath, singleTexturePath, singleTexturePath, singleTexturePath, singleTexturePath });
	}

	void Voxel::setAllVertexData(float x, float y, float z, float width, float height, float depth)
	{
		Color color;
		color.set(255, 255, 255, 255);
		Vertex3D vertexData[36];
		GLuint r = color.r;
		GLuint g = color.g;
		GLuint b = color.b;
		GLuint a = color.a;
		// Top = face 1
		// vertex A
		setVertexData(vertexData[0], x + width, y + height, z + depth, 1.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex B
		setVertexData(vertexData[1], x, y + height, z + depth, 0.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex C
		setVertexData(vertexData[2], x, y + height, z, 0.0f, 0.0f, color.r, color.g, color.b, color.a);

		// vertex C
		setVertexData(vertexData[3], x, y + height, z, 0.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex D
		setVertexData(vertexData[4], x + width, y + height, z, 1.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex A
		setVertexData(vertexData[5], x + width, y + height, z + depth, 1.0f, 1.0f, color.r, color.g, color.b, color.a);

		// Front = face 2
		// vertex D
		setVertexData(vertexData[6], x + width, y + height, z, 1.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex C
		setVertexData(vertexData[7], x, y + height, z, 0.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex E
		setVertexData(vertexData[8], x, y, z, 0.0f, 0.0f, color.r, color.g, color.b, color.a);

		// vertex E
		setVertexData(vertexData[9], x, y, z, 0.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex F
		setVertexData(vertexData[10], x, y + width, z, 1.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex D
		setVertexData(vertexData[11], x + width, y + height, z, 1.0f, 1.0f, color.r, color.g, color.b, color.a);

		// Right = face 3
		// vertex A
		setVertexData(vertexData[12], x + width, y + height, z + depth, 1.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex D
		setVertexData(vertexData[13], x + width, y + height, z, 0.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex F
		setVertexData(vertexData[14], x + width, y, z, 0.0f, 0.0f, color.r, color.g, color.b, color.a);

		// vertex F
		setVertexData(vertexData[15], x + width, y, z, 0.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex G
		setVertexData(vertexData[16], x + width, y, z + depth, 1.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex A
		setVertexData(vertexData[17], x + width, y + height, z + depth, 1.0f, 1.0f, color.r, color.g, color.b, color.a);

		// Back = face 4
		// vertex B
		setVertexData(vertexData[18], x, y + height, z + depth, 1.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex A
		setVertexData(vertexData[19], x + width, y + height, z + depth, 0.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex F
		setVertexData(vertexData[20], x + width, y, z + depth, 0.0f, 0.0f, color.r, color.g, color.b, color.a);

		// vertex F
		setVertexData(vertexData[21], x + width, y, z + depth, 0.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex H
		setVertexData(vertexData[22], x, y, z + depth, 1.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex A
		setVertexData(vertexData[23], x, y + height, z + depth, 1.0f, 1.0f, color.r, color.g, color.b, color.a);

		// Left = face 5
		// vertex C
		setVertexData(vertexData[24], x, y + height, z, 1.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex B
		setVertexData(vertexData[25], x, y + height, z + depth, 0.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex H
		setVertexData(vertexData[26], x, y, z + depth, 0.0f, 0.0f, color.r, color.g, color.b, color.a);

		// vertex H
		setVertexData(vertexData[27], x, y, z + depth, 0.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex E
		setVertexData(vertexData[28], x, y, z, 1.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex C
		setVertexData(vertexData[29], x, y + height, z, 1.0f, 1.0f, color.r, color.g, color.b, color.a);

		// Bottom = face 6
		// vertex F
		setVertexData(vertexData[30], x + width, y, z, 1.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex E
		setVertexData(vertexData[31], x, y, z, 1.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex H
		setVertexData(vertexData[32], x, y, z + depth, 0.0f, 0.0f, color.r, color.g, color.b, color.a);

		// vertex H
		setVertexData(vertexData[33], x, y, z + depth, 0.0f, 0.0f, color.r, color.g, color.b, color.a);
		// vertex G
		setVertexData(vertexData[34], x + width, y, z + depth, 0.0f, 1.0f, color.r, color.g, color.b, color.a);
		// vertex F
		setVertexData(vertexData[35], x + width, y, z, 1.0f, 1.0f, color.r, color.g, color.b, color.a);
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Voxel::loadFaceTextures(const std::string(& texturePaths)[6])
	{
		for (int i = 0; i < 6; i++) 
		{
			_face[i] = ResourceManager::getTexture(texturePaths[i]);
		}
	}

	void Voxel::vboCheck()
	{
		if (_vboID == 0)
		{
			glGenBuffers(1, &_vboID);
		}
	}

	void Voxel::init(float x, float y, float z, float width, float height, float depth, const std::string (&texturePaths)[6])
	{
		loadFaceTextures(texturePaths);
		vboCheck();
		setAllVertexData(x, y, z, width, height, depth);
	}
}
