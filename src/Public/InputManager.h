#pragma once
#include <queue>

#include "KeyboardInput.h"
#include <map>
#include <unordered_map>
#include <functional>
#include <string>
using Callback = std::function<void()>;
using MouseCallback = std::function<void(glm::vec2)>;
using KeyboardInputsMap = std::unordered_map<int, std::map<const void* , Callback>>;
using MouseInputsMap = std::unordered_map<int, std::map<const void* , MouseCallback>>;
using InputsPairKeyboard  = std::pair<const void*, Callback>;
using InputsPairMouse  = std::pair<const void*, MouseCallback>;

class InputManager {

public:
    InputManager(GLFWwindow* context_window);
    ~InputManager() {};
    //Delete copy constructor and assignment operator
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    /* Keyboard */
    void AddKeyboardCallback(int key, KeyboardAction action, const void* ownerPointer, Callback callback);
    void RemoveKeyboardCallback(int key, KeyboardAction action, const void* ownerPointer);
    void QueueKeyboardInput(const KeyboardInput& input);

    /* Mouse */
    void AddMouseCallback(int key, MouseAction action, const void* ownerPointer, const MouseCallback& callback);
    void RemoveMouseCallback(int key, MouseAction action, const void* ownerPointer);
    void QueueMouseInput(const MouseInput& input);

    void ProcessInput();

    bool IsKeyPressed(int key);
    bool IsMouseKeyPressed(int key);
private:
    void Reset();

    const KeyboardInput& UnqueueKeyboardInput();
    const MouseInput& UnqueueMouseInput();

public:
    KeyboardInputsMap* GetKeyboardCallbackFromAction(KeyboardAction action);
    MouseInputsMap * GetMouseCallbackFromAction(MouseAction action);

private:
    /* Keyboard */
    KeyboardInputsMap callbacks_keyboard_pressed;
    KeyboardInputsMap callbacks_keyboard_release;
    KeyboardInputsMap callbacks_keyboard_repeat;


    /* Mouse */
    MouseInputsMap callbacks_mouse_pressed;
    MouseInputsMap callbacks_mouse_release;
    //TODO
    MouseInputsMap callbacks_mouse_moved;

    std::queue<KeyboardInput> keyboardInputs;
    std::queue<MouseInput> MouseInputs;

    GLFWwindow* window;
};
