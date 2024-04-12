#pragma once

#include <iostream>

#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum direction {
	FWD = 0,
	BCK,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
public:
	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp) {
		this->viewMatrix = glm::mat4(1.0f);

		this->movementSpeed = 3.0f;
		this->sensitivity = 0.3f;
		
		this->worldUp = worldUp;
		this->position = position;
		this->right = glm::vec3(0.0f);
		this->up = worldUp;

		this->pitch = 0.0f;
		this->yaw = -90.0f;
		this->roll = 0.0f;

		this->update_cam_vectors();
	}
	~Camera() {

	}
	// accessors
	const glm::mat4 get_view_matrix() {
		this->update_cam_vectors();
		this->viewMatrix = glm::lookAt(this->position, this->position + this->front, this->up);
		return this->viewMatrix;
	}

	const glm::vec3 getPosition() {
		return this->position;
	}

	// functions
	void update_input(const float& delta, const int direction, const double& offsetX, const double& offsetY) {
		this->update_keyboard_input(delta, direction);
		this->update_mouse_input(delta, offsetX, offsetY);
	}

	void move(const float& delta, const int direction) {
		switch (direction) {
		case FWD:
			this->position += this->front * this->movementSpeed * delta;
			break;
		case BCK:
			this->position -= this->front * this->movementSpeed * delta;
			break;
		case LEFT:
			this->position -= this->right * this->movementSpeed * delta;
			break;
		case RIGHT:
			this->position += this->right * this->movementSpeed * delta;
			break;
		case UP:
			this->position += glm::vec3(0.0f, 1.0f, 0.0f) * this->movementSpeed * delta;
			break;
		case DOWN:
			this->position += glm::vec3(0.0f, -1.0f, 0.0f) * this->movementSpeed * delta;
			break;
		default:
			break;
		}
	}

	void update_keyboard_input(const float& delta, const int direction) {

	}

	float get_far_plane()
	{
		return 70.0f;
	}
private:
	glm::mat4 viewMatrix;

	GLfloat movementSpeed;
	GLfloat sensitivity;

	glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat roll;

	void update_cam_vectors() {
		this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front.y = sin(glm::radians(this->pitch));
		this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

		this->front = glm::normalize(this->front);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}

	void update_mouse_input(const float& delta, const double& offsetX, const double& offsetY) {
		this->pitch += static_cast<GLfloat>(-offsetY) * this->sensitivity;
		this->yaw += static_cast<GLfloat>(offsetX) * this->sensitivity;

		if (this->pitch >= 80.0f) this->pitch = 80.0f;
		else if (this->pitch <= -80.0f) this->pitch = -80.0f;

		if (this->yaw > 360 || this->yaw < -360)
			this->yaw = 0.0f;
	}
};