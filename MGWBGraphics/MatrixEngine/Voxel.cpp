#pragma once

#include "Voxel.h"
#include "ResourceManager.h"
#include "Vertex.h"
#include <glm/detail/type_vec4.hpp>
#include <glm/mat4x4.hpp>

#include <glm/gtx/rotate_vector.hpp>

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
			glDrawArrays(GL_TRIANGLES, i * 6, 6);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Voxel::rotate(float angle, glm::vec3 axisInObjectCoord)
	{
		_object2world = glm::rotate(_object2world, glm::degrees(angle), axisInObjectCoord);
		_spatialDiagonal = glm::rotate(_spatialDiagonal, glm::degrees(angle), axisInObjectCoord);
		updateSpatialDiagonalPosition();
		updateVertexPositions();
		setAllVertexData();
		bufferVertexData();
	}

	void Voxel::translate(glm::vec3 v)
	{
		_object2world = glm::translate(_object2world, v);
		updateSpatialDiagonalPosition();
		updateVertexPositions();
		setAllVertexData();
		bufferVertexData();

	}

	void Voxel::resetPosition()
	{
		_object2world = glm::mat4(1.0f);
		updateSpatialDiagonalPosition();
		updateVertexPositions();
		setAllVertexData();
		bufferVertexData();
	}

	std::string Voxel::printPosition()
	{
		std::string pos;

		pos.append(std::to_string(_lbf.x));
		pos.append(",");
		pos.append(std::to_string(_lbf.y));
		pos.append(",");
		pos.append(std::to_string(_lbf.z));
		pos.append(",");
		return pos;
	}

	void Voxel::init(float x, float y, float z, float width, float height, float depth, const std::string singleTexturePath)
	{
		init(x, y, z, width, height, depth, { singleTexturePath, singleTexturePath, singleTexturePath, singleTexturePath, singleTexturePath, singleTexturePath });
	}

	void Voxel::updateVertexPositions()
	{
		Position3D A = { _rtb.x, _rtb.y, _rtb.z };
		Position3D B = { _lbf.x, _rtb.y, _rtb.z };
		Position3D C = { _lbf.x, _rtb.y, _lbf.z };
		Position3D D = { _rtb.x, _rtb.y, _lbf.z };

		Position3D E = { _lbf.x, _lbf.y, _lbf.z };
		Position3D F = { _rtb.x, _lbf.y, _lbf.z };
		Position3D G = { _rtb.x, _lbf.y, _rtb.z };
		Position3D H = { _lbf.x, _lbf.y, _rtb.z };

		_vertices[0] = A;
		_vertices[1] = B;
		_vertices[2] = C;
		_vertices[3] = D;
		_vertices[4] = E;
		_vertices[5] = F;
		_vertices[6] = G;
		_vertices[7] = H;
	}

	void Voxel::updateFaceSetup()
	{
		Face top	=	{ 0, 0, 1, 2, 3 };
		Face front	=	{ 1, 3, 2, 4, 5 };
		Face right	=	{ 2, 0, 3, 5, 6 };
		Face back	=	{ 3, 1, 0, 6, 7 };
		Face left	=	{ 4, 2, 1, 7, 4 };
		Face bottom =	{ 5, 5, 4, 7, 6 };

		_faces[0] = top;
		_faces[1] = front;
		_faces[2] = right;
		_faces[3] = back;
		_faces[4] = left;
		_faces[5] = bottom;
	}

	void Voxel::updateSpatialDiagonalPosition()
	{
		glm::vec4 tmpLbf = glm::vec4(1.0f);
		tmpLbf = _object2world * tmpLbf;
		_lbf = { tmpLbf.x, tmpLbf.y, tmpLbf.z };
		glm::vec4 tmpRtb = tmpLbf + _spatialDiagonal;
		_rtb = { tmpRtb.x, tmpRtb.y, tmpRtb.z };

		/*
		_lbf = { _object2world[3][0], _object2world[3][1], _object2world[3][2] };
		_rtb = {_lbf.x + _dimensions.x, _lbf.y + _dimensions.y, _lbf.z + _dimensions.z};
		*/
	}

	void Voxel::init(float x, float y, float z, float width, float height, float depth, const std::string(&texturePaths)[6])
	{
		_object2world = glm::mat4(1.0f);
		glm::vec3 origin = { x, y, z };
		_object2world = glm::translate(_object2world, origin);
		_dimensions = { width, height, depth };
		_spatialDiagonal = { width, height, depth, 1.0f };
		updateSpatialDiagonalPosition();

		/*_object2world[3][0] = _lbf.x;
		_object2world[3][1] = _lbf.y;
		_object2world[3][2] = _lbf.z;
		*/
		updateFaceSetup();
		updateVertexPositions();
		loadFaceTextures(texturePaths);
		if (_vboID == 0) { glGenBuffers(1, &_vboID); }
		setAllVertexData();
		bufferVertexData();

	}

	void setVertexData(Vertex3D &vertex, Position3D position, UV uv, Color color)
	{
		vertex.position = position;
		vertex.uv = uv;
		vertex.color = color;
	}

	void Voxel::setAllVertexData()
	{
		Color color;
		color.set(255, 255, 255, 255);
		for (int i = 0; i < 6; i++)
		{
			setVertexDataForFace(i, color);
		}
	}

	void Voxel::bufferVertexData()
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_vertexData), _vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Voxel::setVertexDataForFace(int faceId, Color color)
	{
		auto faceOffset = faceId * _FACECOUNT;
		
		setVertexData(_vertexData[0 + faceOffset], _vertices[_faces[faceId].vertices[0]], _TOPRIGHT, color);
		setVertexData(_vertexData[1 + faceOffset], _vertices[_faces[faceId].vertices[1]], _TOPLEFT,	color);
		setVertexData(_vertexData[2 + faceOffset], _vertices[_faces[faceId].vertices[2]], _BOTLEFT, color);
															 
		setVertexData(_vertexData[3 + faceOffset], _vertices[_faces[faceId].vertices[2]], _BOTLEFT, color);
		setVertexData(_vertexData[4 + faceOffset], _vertices[_faces[faceId].vertices[3]], _BOTRIGHT, color);
		setVertexData(_vertexData[5 + faceOffset], _vertices[_faces[faceId].vertices[0]], _TOPRIGHT, color);
	}

	void Voxel::loadFaceTextures(const std::string(&texturePaths)[6])
	{
		for (int i = 0; i < 6; i++)
		{
			_face[i] = ResourceManager::getTexture(texturePaths[i]);
		}
	}
}
