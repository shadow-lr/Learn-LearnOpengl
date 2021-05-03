#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#ifndef GLM_H
#define GLM_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif // !GLM_H

// Construct vector
// Calculate view matrix 
// Keyboard process
// Scroll process
// Get camera front vector

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum CameraMoveType {
	NULLINPUT = 1,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:

	// Construct Vector
	Camera(glm::vec3 _position = glm::vec3(0, 0, 0), glm::vec3 _worldUp = glm::vec3(0, 1.0f, 0),
		GLfloat _yaw = YAW, GLfloat _pitch = PITCH)
		: moveSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
		position = _position;
		worldUp = _worldUp;
		yaw = _yaw;
		pitch = _pitch;
		UpdateCameraVectors();
	}

	// Update camera args(Right Up Front)
	void UpdateCameraVectors()
	{
		glm::vec3 direction;
		direction.x = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
		direction.y = std::sin(glm::radians(pitch));
		direction.z = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));

		// Update front vector
		front = glm::normalize(direction);

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

	// keyBoard Process
	void KeyBoardProcess(CameraMoveType moveType, float deltaTime)
	{
		GLfloat velocity = moveSpeed * deltaTime;
		switch (moveType)
		{
		case FORWARD:
			position += velocity * front;
			break;
		case BACKWARD:
			position -= velocity * front;
			break;
		case LEFT:
			position -= glm::normalize(glm::cross(front, up)) * velocity;
			break;
		case RIGHT:
			position += glm::normalize(glm::cross(front, up)) * velocity;
			break;
		default:
			break;
		}
		//printf("%.2f, %.2f, %.2f\n", position.x, position.y, position.z);
	}

	// Mouse Process
	void MouseProcess(float xoffset, float yoffset)
	{
		// Multiply sensitivity
		xoffset *= mouseSensitivity;
		yoffset *= mouseSensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}

		// yaw or pitch change lead to Up、Right、Front change
		UpdateCameraVectors();
	}

	// Scroll Process
	void ScrollProcess(float yoffset)
	{
		if (zoom >= 1.0f && zoom <= 45.0f)
			zoom -= yoffset;
		if (zoom <= 1.0f)
			zoom = 1.0f;
		if (zoom >= 45.0f)
			zoom = 45.0f;
	}

	// Get camera front vector
	glm::vec3 GetCameraFront()
	{
		return front;
	}

	// Get camera view matrix
	glm::mat4 GetViewMatrix()
	{
		// cameraPosition targetPosition cameraUp
		return glm::lookAt(position, position + front, up);
	}
	
	// Camera variable
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	// Euler
	GLfloat pitch;						// 俯仰角
	GLfloat yaw;						// 偏航角
	// Adjustable options
	GLfloat moveSpeed;					// 鼠标移动速度
	GLfloat mouseSensitivity;			// 鼠标灵敏度
	GLfloat zoom;						// 缩放
};