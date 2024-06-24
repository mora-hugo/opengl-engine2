#pragma once

#include <memory>
#include "Window.h"
#include "InputManager.h"
#include "Chunk/ChunkManager.h"
#include "Camera.h"


namespace HC {

    class App {
    public:
        App();
        void Run();
        bool InitializeOpenGL();
        InputManager * GetInputManagerPointer() { return inputManager.get(); }

    protected:
        void SetupInput();
    private:
        void ShowFPSUsingImGui(double deltaTime);
    private:
        std::unique_ptr<Window> window;
        std::unique_ptr<InputManager> inputManager;
        std::unique_ptr<Camera> camera;
        glm::vec2 lastMousePosition{0.f, 0.f};
        glm::vec3 lastRayCastBlock{0, 0, 0};
        std::unique_ptr<ChunkManager> chunkManager;
    public:


    };

} // HC

