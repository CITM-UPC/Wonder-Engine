#include "Camera.h"
#include "Globals.h"
#include "WonderEngine.h"

#include <glm/ext/matrix_transform.hpp>

Camera::Camera() : fov(60), aspect(4.0 / 3.0), zNear(0.1), zFar(100), eye(10, 2, 10), center(0, 1, 0), up(0, 1, 0) {}

glm::dmat4 Camera::computeLookAt() const {
	return glm::lookAt(eye, center, up);
}

void Camera::cameraMove(int id) {
	computeAxis();
	switch (id) {
	case UP:
		center -= yAxis;
		eye -= yAxis;
		break;
	case DOWN:
		center += yAxis;
		eye += yAxis;
		break;
	case LEFT:
		center -= xAxis;
		eye -= xAxis;
		break;
	case RIGHT:
		center += xAxis;
		eye += xAxis;
		break;
	}
}

void Camera::FPSMovement(int id) {
	computeAxis();
	switch (id) {
	case FORWARD:
		center -= zAxis;
		eye -= zAxis;
		break;
	case BACKWARD:
		center += zAxis;
		eye += zAxis;
		break;
	case LEFT:
		center -= xAxis;
		eye -= xAxis;
		break;
	case RIGHT:
		center += xAxis;
		eye += xAxis;
		break;
	}
}

void Camera::ResetCenter(bool fixedPos, bool changeCenter, vec3 newCenter, bool resetPrevMouse) {
	if (changeCenter) {
		center = newCenter;
	}
	if (fixedPos) {
		eye = normalize(eye);
		eye *= 12;
	}
	if (resetPrevMouse) {
		prevMouseX = 0;
		prevMouseY = 0;
	}
}

void Camera::MouseRotateAroundObject(double x, double y) {
	if (prevMouseX != 0 && prevMouseY != 0) {
		vec2 mouseDir = { x - prevMouseX, y - prevMouseY };
		float angleX = (0.01f * mouseDir.x);
		float angleY = (0.01f * mouseDir.y);
		RotateCameraAroundObject(center, eye, angleY, -xAxis, angleX, yAxis);
	}
	prevMouseX = x;
	prevMouseY = y;
	computeAxis();
}

void Camera::RotateCameraAroundObject(vec3& center, vec3& eye, float angleInRadiansX, const glm::vec3& axisX, float angleInRadiansY, const glm::vec3& axisY) {
	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), angleInRadiansX, axisX);
	vec3 translatedEyeX = eye - center;
	vec3 rotatedEyeX = vec3(rotationX * vec4(translatedEyeX, 1.0f));
	if (glm::normalize(rotatedEyeX + center).y <= 0.99f && glm::normalize(rotatedEyeX + center).y >= -0.99f) {
		eye = rotatedEyeX + center;
	}

	glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), angleInRadiansY, axisY);
	vec3 translatedEyeY = eye - center;
	vec3 rotatedEyeY = vec3(rotationY * vec4(translatedEyeY, 1.0f));
	eye = rotatedEyeY + center;
}

void Camera::MousePointLookAt(double x, double y) {
	if (prevMouseX != 0 && prevMouseY != 0) {
		vec2 mouseDir = { x - prevMouseX, y - prevMouseY };
		float angleX = (0.01f * mouseDir.x);
		float angleY = (0.01f * mouseDir.y);
		RotateCameraFPS(center, eye, angleX, yAxis, angleY, -xAxis);
	}
	prevMouseX = x;
	prevMouseY = y;
	computeAxis();
}

void Camera::RotateCameraFPS(vec3& center, vec3& eye, float angleInRadiansX, const glm::vec3& axisX, float angleInRadiansY, const glm::vec3& axisY) {
	mat4 rotation = rotate(glm::mat4(1.0f), angleInRadiansX, axisX);
	center -= eye;
	center = glm::mat3(rotation) * center;
	center += eye;

	rotation = rotate(glm::mat4(1.0f), angleInRadiansY, axisY);
	center -= eye;
	center = glm::mat3(rotation) * center;
	center += eye;
}

void Camera::CameraZoom(int zoom) {
	vec3 zoomAxis = zAxis;
	zoomAxis *= zoom;
	center -= zoomAxis;
	eye -= zoomAxis;
}

void Camera::computeAxis() {
	zAxis = normalize(eye - center);
	xAxis = normalize(cross(up, zAxis));
	yAxis = normalize(cross(xAxis, zAxis));
	zAxis *= cameraSpeed * 2;
	xAxis *= cameraSpeed;
	yAxis *= cameraSpeed;
}