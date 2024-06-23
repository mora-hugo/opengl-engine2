#pragma once


#include <glm/glm.hpp>
namespace HC {
    enum class CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN
    };

    class Camera {
    public:
        Camera();

        ~Camera();

        glm::mat4 GetMatrix();
        [[nodiscard]] float* GetFovPtr() { return &Fov; }
        [[nodiscard]] float* GetViewDistancePtr(){ return &viewDistance; }
        [[nodiscard]] glm::vec2* GetViewportSizePtr() { return &viewportSize; }
        [[nodiscard]] float* GetMovementSpeedPtr() { return &MovementSpeed; }
        [[nodiscard]] float* GetRotationSpeedPtr() { return &RotationSpeed; }

        [[nodiscard]]float GetFov() const { return Fov; }
        [[nodiscard]]float GetViewDistance() const { return viewDistance; }
        [[nodiscard]] glm::vec3 GetPosition() const { return position; }
        [[nodiscard]] glm::vec3 GetForward() const { return forward; }
        [[nodiscard]] glm::vec3 GetRight() const { return right; }
        [[nodiscard]] glm::vec3 GetUp() const { return up; }
        [[nodiscard]] float GetMovementSpeed() const { return MovementSpeed; }
        [[nodiscard]] float GetRotationSpeed() const { return RotationSpeed; }
        [[nodiscard]]  glm::mat4 GetViewMatrix();
        [[nodiscard]]  glm::mat4 GetProjectionMatrix();
        void SetPosition(const glm::vec3 &pos) { position = pos; }
        void Move(CameraMovement direction);
        void Rotate(const glm::vec2 &rot);
        void UpdateVectors();
        // yaw and pitch
        glm::vec2 rotation{-90.f, 0.f};
    protected:


    protected:
        float Fov = 45.0f;
        float viewDistance = 100.0f;
        float MovementSpeed = 0.01f;
            float RotationSpeed = 0.4f;


        glm::vec2 viewportSize{800.f, 600.f};
        glm::vec3 position{0.f, 0.f, 0.f};
        glm::vec3 forward{0.f, 0.f, 1.f};
        glm::vec3 right{1.f, 0.f, 0.f};
        glm::vec3 up{0.f, 1.f, 0.f};


    };
};
