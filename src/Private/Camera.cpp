
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace HC {
    Camera::Camera() :  forward(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, -1.0f, 0.0f)), position(glm::vec3(0.0f, 0.f, -1.0f))
    {
    }

    glm::mat4 Camera::GetMatrix() {
        return  GetProjectionMatrix() * GetViewMatrix();
    }

    glm::mat4 Camera::GetViewMatrix() {
        return glm::lookAt(position, position + forward, up);
    }

    glm::mat4 Camera::GetProjectionMatrix() {
        return glm::perspective(glm::radians(Fov), viewportSize.x / viewportSize.y, 0.1f, viewDistance);
    }

    Camera::~Camera() {

    }

    void Camera::UpdateVectors() {
        const float yaw = rotation.x;
        const float pitch = rotation.y;
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward = glm::normalize(front);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, glm::vec3(0,1,0)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = glm::normalize(glm::cross(right, forward));

    }

    void Camera::Move(CameraMovement direction) {

        switch (direction) {
            case CameraMovement::FORWARD:
                position += forward * MovementSpeed;
                break;
            case CameraMovement::BACKWARD:
                position -= forward * MovementSpeed;
                break;
            case CameraMovement::LEFT:
                position -= right * MovementSpeed;
                break;
            case CameraMovement::RIGHT:
                position += right * MovementSpeed;
                break;
            case CameraMovement::UP:
                position += up * MovementSpeed;
                break;
            case CameraMovement::DOWN:
                position -= up * MovementSpeed;
                break;
        }
        UpdateVectors();
    }

    void Camera::Rotate(const glm::vec2 &rot) {
        rotation += rot * RotationSpeed;
        if(rotation.y > 89.0f)
            rotation.y = 89.0f;
        if(rotation.y < -89.0f)
            rotation.y = -89.0f;
        UpdateVectors();
    }


}