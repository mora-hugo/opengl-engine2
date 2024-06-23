#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <chrono>
#include <valarray>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "App.h"
#include "Utils.h"
#include "Shader.h"
#include "Program.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Model.h"
#include "Shaders.h"
#include "Chunk/Chunk.h"
#include "Tests/Test.h"
namespace HC {
    bool HC::App::InitializeOpenGL() {
        return gladLoadGL();
    }

    void App::Run() {
        if (!InitializeOpenGL()) {
            throw std::runtime_error("Failed to initialize OpenGL");
        }
        glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        //Max fps
        Test::testIndices();
        glfwSwapInterval(1);
        Shaders shaders("../src/Shaders/vertex.glsl", "../src/Shaders/fragment.glsl");

        Program program;
        program.AttachShader(shaders.GetVertexShaderId());
        program.AttachShader(shaders.GetFragmentShaderId());
        program.Link();


        CompileStatus compileStatus;
        program.GetCompileStatus(compileStatus);
        if (!compileStatus.success) {
            throw std::runtime_error("Failed to link program");
        }

        program.Use();

        program.DeleteShader(shaders.GetVertexShaderId());
        program.DeleteShader(shaders.GetFragmentShaderId());

        std::shared_ptr<TextureData> textureData = Model::LoadTexture("../resources/textures/container.jpg");
        GLuint TextureId = Model::CREATE_TEXTURE(textureData);
        Model::BIND_TEXTURE(TextureId);

        glm::mat4 trans = glm::mat4(1.0f);

        Chunk chunk;
        chunk.Initialize();
        SetupInput();


        textureData->Free();

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto previousTime = currentTime;
        float deltaTime = 0.f;
        while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
            glfwPollEvents();

            inputManager->ProcessInput();
            window->ImGUIFrameBegin();

            ShowFPSUsingImGui(deltaTime);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            program.Use();
            chunk.Draw();

            window->ImGUIRender();

            glfwSwapBuffers(window->GetGLFWWindow());

            /* Calculate delta time */
            deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
            previousTime = currentTime;
            currentTime = std::chrono::high_resolution_clock::now();

            program.SetUniformMat4("view_proj", camera->GetMatrix());
            program.SetUniformMat4("model", trans);



        }

    }

    App::App() {
        window = std::make_unique<Window>(800, 600, "OpenGL Test 2");
        inputManager = std::make_unique<InputManager>(window->GetGLFWWindow());
        camera = std::make_unique<Camera>();
        window->SetAppContext(this);

    }

    void App::ShowFPSUsingImGui(double deltaTime) {
        static float fps = 0.f;
        static float frameTime = 0.f;
        static float frameTimeAverage = 0.f;
        static std::valarray<float> frameTimes(100);
        static int frameTimeIndex = 0;

        frameTime = 1.f / ImGui::GetIO().Framerate;
        frameTimes[frameTimeIndex] = frameTime;
        frameTimeIndex = (frameTimeIndex + 1) % frameTimes.size();
        frameTimeAverage = frameTimes.sum() / frameTimes.size();
        fps = ImGui::GetIO().Framerate;

        ImGui::Begin("Engine parameters/info");
        ImGui::Text("FPS: %.f", 1/deltaTime);

        if(ImGui::CollapsingHeader("Camera")) {
            ImGui::SliderFloat("Fielf of view", camera->GetFovPtr(), 10.f, 180.f);
            ImGui::SliderFloat("View distance", camera->GetViewDistancePtr(), 1.f, 10000.f);
            ImGui::SliderFloat("Movement Speed", camera->GetMovementSpeedPtr(), 0.01f, 1.f);
            ImGui::SliderFloat("Rotation Speed", camera->GetRotationSpeedPtr(), 0.01f, 1.f);
        }

        ImGui::End();
    }

    void App::SetupInput() {
        inputManager->AddKeyboardCallback(GLFW_KEY_ESCAPE, KeyboardAction::VP_KEY_PRESSED, this, [&]() {
            glfwSetWindowShouldClose(window->GetGLFWWindow(), true);
        });

        inputManager->AddKeyboardCallback(GLFW_KEY_F, KeyboardAction::VP_KEY_PRESSED, this, [&](){
            double xPos, yPos;
            glfwGetCursorPos(window->GetGLFWWindow(), &xPos, &yPos);
            lastMousePosition = glm::vec2(xPos, yPos);
            if(glfwGetInputMode(window->GetGLFWWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
                glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        });

        inputManager->AddKeyboardCallback(GLFW_KEY_S, KeyboardAction::VP_KEY_REPEAT, this, [&]() {
            camera->Move(CameraMovement::BACKWARD);
        });

        inputManager->AddKeyboardCallback(GLFW_KEY_W, KeyboardAction::VP_KEY_REPEAT, this, [&]() {
            camera->Move(CameraMovement::FORWARD);
        });

        inputManager->AddKeyboardCallback(GLFW_KEY_A, KeyboardAction::VP_KEY_REPEAT, this, [&]() {
            camera->Move(CameraMovement::LEFT);
        });

        inputManager->AddKeyboardCallback(GLFW_KEY_D, KeyboardAction::VP_KEY_REPEAT, this, [&]() {
            camera->Move(CameraMovement::RIGHT);
        });

        inputManager->AddKeyboardCallback(GLFW_KEY_E, KeyboardAction::VP_KEY_REPEAT, this, [&]() {
            camera->Move(CameraMovement::UP);
        });

        inputManager->AddKeyboardCallback(GLFW_KEY_Q, KeyboardAction::VP_KEY_REPEAT, this, [&]() {
            camera->Move(CameraMovement::DOWN);
        });

        inputManager->AddMouseCallback(0, MouseAction::VP_MOUSE_MOVE, this, [&](glm::vec2 pos) {
            if(glfwGetInputMode(window->GetGLFWWindow(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
                return;
            }
            glm::vec2 delta = pos - lastMousePosition;
            lastMousePosition = pos;
            delta.y *= -1;
            camera->Rotate(delta);
        });
    }






} // HC