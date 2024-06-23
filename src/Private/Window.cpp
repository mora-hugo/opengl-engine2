//
// Created by hugom on 25/12/2023.
//

#include "Window.h"
#include "App.h"

#include "glm/vec2.hpp"

/* IMGUI */
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <memory>
#include <stdexcept>
#include <iostream>

namespace HC {

    Window::Window(int width, int height, const std::string &windowName) : windowSize(width, height), windowName(windowName) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_callback);
        glfwSetCursorPosCallback(window, mouse_position_callback);

        InitializeIMGUI();
    }

    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        auto *windowRef = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        if(!windowRef) return;

        windowRef->windowSize.x = width;
        windowRef->windowSize.y = height;

        glViewport(0, 0, width, height);


    }

    void Window::InitializeIMGUI() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();


        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);

        //ImGui_ImplOpenGL3_Init("#version 330");
    }

    void Window::ImGUIFrameBegin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Window::ImGUIRender() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        Window *vp_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if(!vp_window) return;

        KeyboardInput input(key, scancode, action);
        vp_window->appContext->GetInputManagerPointer()->QueueKeyboardInput(input);

    }

    void Window::mouse_callback(GLFWwindow *window, int button, int action, int mods) {
        Window *vp_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if(!vp_window) return;

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        glm::vec2 mousePos {mouseX, mouseY};


        MouseInput input(button, action,mousePos);
        vp_window->appContext->GetInputManagerPointer()->QueueMouseInput(input);
    }

    void Window::mouse_position_callback(GLFWwindow *window, double xpos, double ypos) {
        Window *vp_window = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if(!vp_window) return;

        glm::vec2 mousePos {xpos, ypos};

        //TODO
        MouseInput input(0, MouseInput::MOUSE_MOVE_INT, mousePos);
        vp_window->appContext->GetInputManagerPointer()->QueueMouseInput(input);
    }


} // HC