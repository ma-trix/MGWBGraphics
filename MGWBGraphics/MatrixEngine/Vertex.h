#pragma once

#include <GL/glew.h>
namespace MatrixEngine
{
	struct Color {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct Position
	{
		float x;
		float y;
	};

	struct Position3D
	{
		float x;
		float y;
		float z;
	};

	struct UV
	{
		float u;
		float v;
	};

	struct Vertex
	{
		Position position;
		Color color;
		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}
	};

	struct Vertex3D
	{
		Position3D position;
		Color color;
		UV uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}

		void setPosition(float x, float y, float z)
		{
			position.x = x;
			position.y = y;
			position.z = z;
		}
	};
}