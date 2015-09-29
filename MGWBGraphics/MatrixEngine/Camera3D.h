#pragma once
#include <glm/glm.hpp>

namespace MatrixEngine {
	class Camera3D
	{
	public:
		Camera3D();
		~Camera3D();
		void createFrustumMatrix(float left, float right, float bottom, float top, float near, float far);
		void init(int screenWidth, int screenHeight, float foV, float aspectRatio, float front, float back);
		void update();

		glm::vec3 convertScreenToWorld(glm::vec3 screenCoords);

		void setPosition(const glm::vec3& newPosition)
		{
			_position = newPosition;
			_needsMatrixUpdate = true;
		}

		void zoom(int mouseWheelMotion);
		void setScale(float newScale)
		{
			_scale = newScale;
			_needsMatrixUpdate = true;
		};

		glm::vec3 getPosition()	{ return _position; }
		float getScale() { return _scale; }
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }
		glm::mat4 getProjectionMatrix() { return _frustumMatrix; };
		glm::mat4 getLookAtMatrix() { return _lookAtMatrix; }
		glm::vec3 getArcBallVector(glm::vec2 mouse);

	private:
		const double _DEG2RAD = 3.14159265 / 180;
		const float _MINFOV = 1.0f;
		const float _MAXFOV = 45.0f;
		glm::mat4 _lookAtMatrix;
		void Camera3D::createFrustumMatrix();
		void createLookAtMatrix();
		void calculateProjection();
		int _screenWidth;
		int _screenHeight;
		bool _needsMatrixUpdate;
		float _scale;
		glm::vec3 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
		glm::mat4 _frustumMatrix;
		double _tangent;
		double _height;
		double _width;
		float _fov;
		float _front;
		float _back;
		float _aspectRatio;

		float normalizeMouse(int coord, int dimensionSize);
		glm::vec3 Camera3D::mouseToScreenCoords(glm::vec2 mouseCoords);
	};
}