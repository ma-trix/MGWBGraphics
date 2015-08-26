#include "Voxel.h"
#include "ResourceManager.h"
#include "Vertex.h"


namespace MatrixEngine {
	Voxel::Voxel()
	{
		_vboID = 0;
	}


	Voxel::~Voxel()
	{
		if (_vboID != 0)
		{
			glDeleteBuffers(1, &_vboID);
		}
	}

	void Voxel::draw()
	{
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, _vboID); //TODO: added via comparison to Sprite, not sure if it's correct

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, uv));

		glBindTexture(GL_TEXTURE_2D, _face[0].id);
		
		glDrawArrays(GL_TRIANGLES, 6, 6);
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

	void setTriangleData()
	{
		// counter clockwise vertex order
	}

	void setSquareData(float x, float y, float z, float width, float height, float depth)
	{
		// upper left triangle
		// lower right triangle
	}

	void Voxel::init(float x, float y, float z, float width, float height, float depth, std::string texturePath)
	{
		_x = x;
		_y = y;
		_z = z;
		_width = width;
		_height = height;
		_depth = depth;

		for (int i = 0; i < 6; i++) {
			_face[i] = ResourceManager::getTexture(texturePath);
		}

		if (_vboID == 0)
		{
			glGenBuffers(1, &_vboID);
		}

		Vertex3D vertexData[36];
		GLuint r = 255;
		GLuint g = 255;
		GLuint b = 255;
		GLuint a = 255;
		// Top = face 1
		// vertex A
		setVertexData(vertexData[0], _x + _width, _y + _height, _z - _depth, 1.0f, 1.0f, r, g, b, a);
		// vertex B
		setVertexData(vertexData[1], x, y + height, z - depth, 0.0f, 1.0f, r, g, b, a);
		// vertex C
		setVertexData(vertexData[2], x, y + height, z, 0.0f, 0.0f, r, g, b, a);
		
		// vertex C
		setVertexData(vertexData[3], x, y + height, z, 0.0f, 0.0f, r, g, b, a);
		// vertex D
		setVertexData(vertexData[4], x + width, y + height, z, 1.0f, 0.0f, r, g, b, a);
		// vertex A
		setVertexData(vertexData[5], x + width, y + height, z - depth, 1.0f, 1.0f, r, g, b, a);
		
		// Front = face 2
		// vertex D
		setVertexData(vertexData[6], x + width, y + height, z, 1.0f, 1.0f, r, g, b, a);
		// vertex C
		setVertexData(vertexData[7], x, y + height, z, 0.0f, 1.0f, r, g, b, a);
		// vertex E
		setVertexData(vertexData[8], x, y, z, 0.0f, 0.0f, r, g, b, a);

		// vertex E
		setVertexData(vertexData[9], x, y, z, 0.0f, 0.0f, r, g, b, a);
		// vertex F
		setVertexData(vertexData[10], x, y + width, z, 1.0f, 0.0f, r, g, b, a);
		// vertex D
		setVertexData(vertexData[11], x + width, y + height, z, 1.0f, 1.0f, r, g, b, a);

		// Right = face 3
		// vertex A
		setVertexData(vertexData[12], x + width, y + height, z + depth, 1.0f, 1.0f, r, g, b, a);
		// vertex D
		setVertexData(vertexData[13], x + width, y + height, z, 0.0f, 1.0f, r, g, b, a);
		// vertex F
		setVertexData(vertexData[14], x + width, y, z, 0.0f, 0.0f, r, g, b, a);
		
		// vertex F
		setVertexData(vertexData[15], x + width, y, z, 0.0f, 0.0f, r, g, b, a);
		// vertex G
		setVertexData(vertexData[16], x + width, y, z + depth, 1.0f, 0.0f, r, g, b, a);
		// vertex A
		setVertexData(vertexData[17], x + width, y + height, z + depth, 1.0f, 1.0f, r, g, b, a);

		// Back = face 4
		// vertex B
		setVertexData(vertexData[18], x, y + height, z + depth, 1.0f, 1.0f, r, g, b, a);
		// vertex A
		setVertexData(vertexData[19], x + width, y + height, z + depth, 0.0f, 1.0f, r, g, b, a);
		// vertex F
		setVertexData(vertexData[20], x + width, y, z + depth, 0.0f, 0.0f, r, g, b, a);

		// vertex F
		setVertexData(vertexData[21], x + width, y, z + depth, 0.0f, 0.0f, r, g, b, a);
		// vertex H
		setVertexData(vertexData[22], x, y, z + depth, 1.0f, 0.0f, r, g, b, a);
		// vertex A
		setVertexData(vertexData[23], x, y + height, z + depth, 1.0f, 1.0f, r, g, b, a);

		// Left = face 5
		// vertex C
		setVertexData(vertexData[24], x, y + height, z, 1.0f, 1.0f, r, g, b, a);
		// vertex B
		setVertexData(vertexData[25], x, y + height, z + depth, 0.0f, 1.0f, r, g, b, a);
		// vertex H
		setVertexData(vertexData[26], x, y, z + depth, 0.0f, 0.0f, r, g, b, a);

		// vertex H
		setVertexData(vertexData[27], x, y, z + depth, 0.0f, 0.0f, r, g, b, a);
		// vertex E
		setVertexData(vertexData[28], x, y, z, 1.0f, 0.0f, r, g, b, a);
		// vertex C
		setVertexData(vertexData[29], x, y + height, z, 1.0f, 1.0f, r, g, b, a);

		// Bottom = face 6
		// vertex F
		setVertexData(vertexData[30], x + width, y, z, 1.0f, 1.0f, r, g, b, a);
		// vertex E
		setVertexData(vertexData[31], x, y, z, 1.0f, 0.0f, r, g, b, a);
		// vertex H
		setVertexData(vertexData[32], x, y, z + depth, 0.0f, 0.0f, r, g, b, a);

		// vertex H
		setVertexData(vertexData[33], x, y, z + depth, 0.0f, 0.0f, r, g, b, a);
		// vertex G
		setVertexData(vertexData[34], x + width, y, z + depth, 0.0f, 1.0f, r, g, b, a);
		// vertex F
		setVertexData(vertexData[35], x + width, y, z, 1.0f, 1.0f, r, g, b, a);

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
