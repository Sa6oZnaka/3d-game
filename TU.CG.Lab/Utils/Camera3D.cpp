#include "Camera3D.hpp"

#include <algorithm>
#include <glm/ext/matrix_transform.hpp>
#include <cmath>

namespace Utils
{
	Camera3D::Camera3D(
		const glm::vec3 position, const glm::vec3 worldUp, const float maxZoom)
		:
		position(5.0f, 2.0f, 7.0f), front(0.0f, 0.0f, -1.0f), up(), right(), worldUp(worldUp),
		yaw(-90.0f), pitch(0.0f), movementSpeed(movementSpeedWalk), mouseSensitivity(0.1f),
		previousCursorPosition(0.0f), zoom(maxZoom), maxZoom(maxZoom),
		isUserControlEnabled(true)
	{
	}

	glm::mat4 Camera3D::GetViewMatrix() const
	{
		return glm::lookAt(position, position + front, up);
	}

	bool Camera3D::AllowedPos(glm::vec3 pos) {
		//map

		int posX = round(pos.x);
		int posY = round(pos.y);
		int posZ = round(pos.z);

		if (map[(int)posX][(int)posZ][(int)posY] != 0)
			return false;

		if (map[(int)posX][(int)posZ][(int)posY - 1] != 0)
			return false;

		if (pos.x > 1 && pos.x < 31 && pos.z > 1 && pos.z < 31)
			return true;

		return false;
	}

	void Camera3D::SetMap(int data[32][32][32]) {
		for (int x = 0; x < 32; x++) {
			for (int y = 0; y < 32; y++) {
				for (int z = 0; z < 32; z++) {
					map[x][y][z] = data[x][y][z];
				}
			}
		}
	}

	float Camera3D::GetY(glm::vec3 pos) {
		int posX = round(pos.x);
		int posY = round(pos.y);
		int posZ = round(pos.z);

		if (map[(int)posX][(int)posZ][(int)posY - 2] == 0) {
			falling = true;
			return pos.y -= 0.1f;
		}

		falling = false;
		return pos.y;
	}

	void Camera3D::Update(const float deltaTime, Input::InputManager& inputManager)
	{
		if (!isUserControlEnabled)
			return;

		const auto velocityLeftRight = movementSpeedWalk * deltaTime;
		const auto velocityForwardBackward = movementSpeed * deltaTime;
		const auto forwardBackwardMovement = front * velocityForwardBackward;
		const auto leftRightMovement = (right * velocityLeftRight);

		position.y = GetY(position);

		if (inputManager.IsKeyDown(Input::Keys::W)) {
			//glm::vec3 nextPos = position + forwardBackwardMovement;
			auto nextPos = position;
			nextPos.x = position.x + forwardBackwardMovement.x;
			nextPos.z = position.z + forwardBackwardMovement.z;

			if (AllowedPos(nextPos))
				position = nextPos;
		}
		if (inputManager.IsKeyDown(Input::Keys::S)) {
			auto nextPos = position;
			nextPos.x = position.x - forwardBackwardMovement.x;
			nextPos.z = position.z - forwardBackwardMovement.z;

			if (AllowedPos(nextPos))
				position = nextPos;
		}
		if (inputManager.IsKeyDown(Input::Keys::A)) {
			glm::vec3 nextPos = position - leftRightMovement;
			if (AllowedPos(nextPos))
				position = nextPos;
		}
		if (inputManager.IsKeyDown(Input::Keys::D)) {
			glm::vec3 nextPos = position + leftRightMovement;
			if (AllowedPos(nextPos))
				position = nextPos;
		}

		if (inputManager.IsKeyDown(Input::Keys::SPACE) && !falling) {
			glm::vec3 nextPos = position;
			nextPos.y += 1;

			if (AllowedPos(nextPos))
				position = nextPos;
		}

		if (inputManager.IsKeyDown(Input::Keys::LEFT_CONTROL))
			movementSpeed = movementSpeedRun;
		else
			movementSpeed = movementSpeedWalk;

		const auto currentCursorPos = inputManager.GetCursorPosition();

		auto xOffset = currentCursorPos.x - previousCursorPosition.x;
		auto yOffset = previousCursorPosition.y - currentCursorPos.y;

		previousCursorPosition = currentCursorPos;

		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		yaw += xOffset;
		pitch += yOffset;

		pitch = std::clamp(pitch, -89.0f, 89.0f);

		const auto yawRadians = glm::radians(yaw);
		const auto pitchRadians = glm::radians(pitch);

		front.x = cos(yawRadians) * cos(pitchRadians);
		front.y = sin(pitchRadians);
		front.z = sin(yawRadians) * cos(pitchRadians);

		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));

		zoom -= inputManager.GetScrollValue();
		zoom = std::clamp(zoom, 1.0f, maxZoom);
	}
}
