#include "DisplayHelper.h"
#include <GL/glew.h>
#include <MatrixEngine/Vertex.h>
#include <stddef.h>

void DisplayHelper::drawAxes()
{
		GLuint _vboID = 0;
		GLuint _vao = 0;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 3.0f;
		float h = 3.0f;
		float d = 3.0f;

		if (_vboID == 0)
		{
			glGenBuffers(1, &_vboID);
		}

		MatrixEngine::Vertex3D vertexData[6];

		vertexData[0].setPosition(x, y, z);
		vertexData[1].setPosition(x + w, y, z);

		vertexData[2].setPosition(x, y, z);
		vertexData[3].setPosition(x, y + h, z);

		vertexData[4].setPosition(x, y, z);
		vertexData[5].setPosition(x, y, z + w);

		vertexData[0].setColor(255, 0, 0, 255);
		vertexData[1].setColor(255, 0, 0, 255);
		vertexData[2].setColor(0, 255, 0, 255);
		vertexData[3].setColor(0, 255, 0, 255);
		vertexData[4].setColor(0, 0, 255, 255);
		vertexData[5].setColor(0, 0, 255, 255);

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		//unbinding buffer, 0 means no buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		glBindBuffer(GL_ARRAY_BUFFER, _vboID);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MatrixEngine::Vertex3D), (void*)offsetof(MatrixEngine::Vertex3D, position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(MatrixEngine::Vertex3D), (void*)offsetof(MatrixEngine::Vertex3D, color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(MatrixEngine::Vertex3D), (void*)offsetof(MatrixEngine::Vertex3D, uv));

		glDrawArrays(GL_LINES, 0, 6);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
}