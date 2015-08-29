#include "Camera3D.h"
#include <glm/gtc/matrix_transform.inl>


namespace MatrixEngine {
	Camera3D::Camera3D() : 
		_screenWidth(1600), 
		_screenHeight(900),
		_needsMatrixUpdate(true),
		_scale(1.0f),
		_position(0.0f, 0.0f, 0.0f),
		_orthoMatrix(1.0f),
		_frustumMatrix(1.0f)
	{
	}


	Camera3D::~Camera3D()
	{
	}

	void Camera3D::createFrustumMatrix(float left, float right, float bottom, float top, float near, float far)
	{
		_frustumMatrix[0][0] = 2 * near / (right - left);			//m0
		_frustumMatrix[0][2] = (right + left) / (right - left);		//m2
		_frustumMatrix[1][1] = 2 * near / (top - bottom);			//m5
		_frustumMatrix[1][2] = (top + bottom) / (top - bottom);		//m6
		_frustumMatrix[2][2] = -(far + near) / (far - near);		//m10
		_frustumMatrix[2][3] = -(2 * far * near) / (far - near);	//m11
		_frustumMatrix[3][2] = -1;									//m14
		_frustumMatrix[3][3] = 0;									//m15
		//TODO: Do I need to transpose the frustum matrix?
		// _frustumMatrix = glm::transpose(_frustumMatrix); 
	}

	void Camera3D::createFrustumMatrix()
	{
		createFrustumMatrix(-_width, _width, -_height, _height, _front, _back);
	}

	void Camera3D::init(int screenWidth, int screenHeight, float foV, float aspectRatio, float front, float back)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
		_fov = foV;
		_aspectRatio = aspectRatio;
		_front = front;
		_back = back;
		calculateProjection();
		
		createFrustumMatrix(-_width, _width, -_height, _height, _front, _back);
	}

	void Camera3D::update()
	{
		//TODO: figure out the exact way to use camera matrix in the vertex shader
		if (_needsMatrixUpdate)
		{
			// Translate
			/*glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, -_position.z);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);*/
			//TODO: Check what is the proper value of Z for translation.
			//glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			//_cameraMatrix = glm::translate(_cameraMatrix, translate);
			// Scale
			glm::vec3 scale(_scale, _scale, _scale);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_needsMatrixUpdate = false;
		}
	}

	glm::vec3 Camera3D::convertScreenToWorld(glm::vec3 screenCoords)
	{
		// Invert Y direction
		screenCoords.y = _screenHeight - screenCoords.y;
		// Centre on 0 
		//TODO: Check what is the proper value of Z for screen coordinates
		screenCoords -= glm::vec3(_screenWidth / 2, _screenHeight / 2, 0.0f);
		// Scale
		screenCoords /= _scale;
		// Translate
		screenCoords += _position;
		return screenCoords;
	}

	void Camera3D::zoom(int mouseWheelMotion)
	{
		_fov += mouseWheelMotion;
		if (_fov >= _MINFOV && _fov <= _MAXFOV)
		{
			calculateProjection();
			createFrustumMatrix();
			_needsMatrixUpdate = true;
		}
		if (_fov < _MINFOV)
		{
			_fov = _MINFOV;
		}
		if (_fov > _MAXFOV)
		{
			_fov = _MAXFOV;
		}
		
	}

	void Camera3D::calculateProjection()
	{
		_tangent = tan(_fov / 2 * _DEG2RAD);
		_height = _front * _tangent;
		_width = _height * _aspectRatio;
	}

	glm::vec3 Camera3D::getArcBallVector(glm::vec2 mouse)
	{
		glm::vec3 P = glm::vec3(1.0f * mouse.x / _screenWidth * 2 - 1.0f, 1.0f * mouse.y / _screenHeight * 2 - 1.0f, 0.0f);
		P.y = -P.y;
		float OP_squared = P.x * P.x + P.y * P.y;
		if(OP_squared <= 1*1)
		{
			P.z = sqrt(1 * 1 - OP_squared);
		}
		else
		{
			P = glm::normalize(P);
		}
		return P;
	}
}