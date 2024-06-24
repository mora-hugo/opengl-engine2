#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <chrono>
#include <valarray>

#include <glm/glm.hpp>


#include "App.h"
#include "Shader.h"
#include "Program.h"
#include "imgui.h"
#include "Model.h"
#include "Shaders.h"
#include "Tests/Test.h"
#include "Guizmo.h"
#include "Chunk/ChunkManager.h"

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
        SetupInput();
        chunkManager->Initialize();


        textureData->Free();

        auto currentTime = std::chrono::high_resolution_clock::now();
        auto previousTime = currentTime;
        float deltaTime = 0.f;
        program.Use();
        while (!glfwWindowShouldClose(window->GetGLFWWindow())) {
            glfwPollEvents();

            inputManager->ProcessInput();

            ShowImGui(deltaTime);


            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            chunkManager->Draw();

            if(!(bBlockDetected = chunkManager->ChunkRayCast(camera->GetPosition(), camera->GetForward(), interactionDistance, lastRayCastBlock, lastHitBlockNormal))) {
                lastRayCastBlock = glm::vec3(0, 0, 0);
                lastHitBlockNormal = glm::vec3(0, 0, 0);
            }

            window->ImGUIRender();
            ShowGizmo();


            glfwSwapBuffers(window->GetGLFWWindow());

            /* Calculate delta time */
            deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
            previousTime = currentTime;
            currentTime = std::chrono::high_resolution_clock::now();

            program.SetUniformMat4("view_proj", camera->GetMatrix());
            program.SetUniformMat4("model", trans);



        }

    }

    App::App(){
        window = std::make_unique<Window>(800, 600, "OpenGL Test 2");
        inputManager = std::make_unique<InputManager>(window->GetGLFWWindow());
        camera = std::make_unique<Camera>();
        window->SetAppContext(this);
        chunkManager = std::make_unique<ChunkManager>();

    }

    void App::ShowImGui(double deltaTime) {
        window->ImGUIFrameBegin();
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

        if(ImGui::CollapsingHeader("Guizmo")) {
            ImGui::Checkbox("Show Gizmo", &bShowGizmo);
            if(bShowGizmo) {
                ImGui::Checkbox("Show Block Wireframe", &bShowBlockWireframe);
                ImGui::Checkbox("Show Normals", &bShowNormals);
                ImGui::Checkbox("Show Line Trace", &bShowLineTrace);
            }
        }

        if(ImGui::CollapsingHeader("Chunk Manager")) {
            ImGui::Text("Chunks: %d", chunkManager->GetChunkCount());
        }

        if(ImGui::CollapsingHeader("Interaction")) {
            ImGui::SliderFloat("Interaction Distance", &interactionDistance, 1.f, 100.f);
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

        inputManager->AddMouseCallback(GLFW_MOUSE_BUTTON_LEFT, MouseAction::VP_MOUSE_PRESSED, this, [&](glm::vec2 offset) {
            chunkManager->SetBlockAt(lastRayCastBlock, 0);
        });

        inputManager->AddMouseCallback(GLFW_MOUSE_BUTTON_RIGHT, MouseAction::VP_MOUSE_PRESSED, this, [&](glm::vec2 offset) {
            chunkManager->SetBlockAt(lastRayCastBlock+lastHitBlockNormal, 1);
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

    void App::ShowGizmo() {
        if(!bShowGizmo) {
            return;
        }
        if(bShowBlockWireframe && bBlockDetected) {
            Guizmo::GetInstance()->DrawCube(lastRayCastBlock, lastRayCastBlock + glm::vec3({1.f, 1.f, 1.f}), {1.f, 0.f, 0.f});
        }
        if(bShowNormals) {
            Guizmo::GetInstance()->DrawLine((lastRayCastBlock+glm::vec3{0.5f, 0.5f, 0.5f}+lastHitBlockNormal), (lastRayCastBlock +glm::vec3{0.5f, 0.5f, 0.5f}) + (lastHitBlockNormal*0.f), {0.f, 1.f, 0.f});
        }
        if(bShowLineTrace) {
            Guizmo::GetInstance()->DrawLine(camera->GetPosition()+glm::vec3(0,-0.1,0), camera->GetPosition() + camera->GetForward() * 10.f, {1.f, 0.f, 0.f});
        }

        Guizmo::GetInstance()->ProcessDraw();
    }


} // HC