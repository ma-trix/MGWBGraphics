#pragma once

#include "Voxel.h"
#include "ResourceManager.h"
#include "Vertex.h"
#include <glm/detail/type_vec4.hpp>
#include <glm/mat4x4.hpp>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

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
		// angleAxis(degrees(RotationAngle), RotationAxis)
		glm::quat q = glm::angleAxis(glm::degrees(angle), axisInObjectCoord);
		rotate(q);
//		_object2world = glm::rotate(_object2world, glm::degrees(angle), axisInObjectCoord);
//		
//		updateVertexPositions();
//		setAllVertexData();
//		bufferVertexData();
	}

	void Voxel::translate(glm::vec3 v)
	{
		_translationM = glm::translate(_translationM, v);
		O2wNeedsUpdate();
		updateVertexPositions();
		setAllVertexData();
		bufferVertexData();
	}

	void Voxel::setDefaultPosition()
	{
		_orientation = glm::quat(glm::vec3(0, 0, 0));
		_rotationM = glm::toMat4(_orientation);
		_translationM = glm::translate(glm::mat4(1.0f), _origin);
		_object2world = _translationM * _rotationM;
	}

	void Voxel::resetPosition()
	{
		setDefaultPosition();
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

	void Voxel::init(float x, float y, float z, float width, float height, float depth, const std::string texPath)
	{
		init(x, y, z, width, height, depth, { texPath, texPath, texPath, texPath, texPath, texPath });
	}

	//TODO: Vertex positions should be updated on the GPU?
	void Voxel::updateVertexPositions()
	{
		glm::vec4 lbf = {
			_origin.x - (_dimensions.x * 0.5f),
			_origin.y - (_dimensions.y * 0.5f),
			_origin.z - (_dimensions.z * 0.5f),
			1 };
		
		glm::vec4 rtb = {
			_origin.x + (_dimensions.x * 0.5f),
			_origin.y + (_dimensions.y * 0.5f),
			_origin.z + (_dimensions.z * 0.5f),
			1 };

		lbf = getObject2world() * lbf;
		rtb = getObject2world() * rtb;

		//TODO: remove _lbf or do something else with them to remove duplication
		_lbf = { lbf.x, lbf.y, lbf.z };
		//TODO: remove _rtb
		_rtb = { rtb.x, rtb.y, rtb.z };

		Position3D A = { rtb.x, rtb.y, rtb.z };
		Position3D B = { lbf.x, rtb.y, rtb.z };
		Position3D C = { lbf.x, rtb.y, lbf.z };
		Position3D D = { rtb.x, rtb.y, lbf.z };

		Position3D E = { lbf.x, lbf.y, lbf.z };
		Position3D F = { rtb.x, lbf.y, lbf.z };
		Position3D G = { rtb.x, lbf.y, rtb.z };
		Position3D H = { lbf.x, lbf.y, rtb.z };

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

	void Voxel::init(float x, float y, float z, float width, float height, float depth, const std::vector<std::string> &texPaths)
	{
		_origin = { x, y, z };
		_dimensions = { width, height, depth };
		setDefaultPosition();
		updateFaceSetup();
		updateVertexPositions();
		loadFaceTextures(_FACECOUNT, texPaths);
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

	void Voxel::loadFaceTextures(int numFaces, const std::vector<std::string> & texPaths)
	{
		for (int i = 0; i < numFaces; i++)
		{
			_face[i] = ResourceManager::getTexture(texPaths[i]);
		}
	}

	void Voxel::rotate(glm::quat rotation)
	{
		glm::quat normalized = glm::normalize(rotation);
		_orientation = normalized  * _orientation;
		_rotationM = glm::toMat4(_orientation);
		O2wNeedsUpdate();
		updateVertexPositions();
		setAllVertexData();
		bufferVertexData();
	}
}
