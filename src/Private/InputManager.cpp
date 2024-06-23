//
// Created by hugom on 15/12/2023.
//

#include "InputManager.h"

void InputManager::AddKeyboardCallback(int key, KeyboardAction action, const void* ownerPointer, Callback callback) {
    KeyboardInputsMap * callbacks = GetKeyboardCallbackFromAction(action);
    if(callbacks->find(key) == callbacks->end()) {
        callbacks->insert({});
    }
    (*callbacks)[key].insert({ownerPointer, callback});
}

void InputManager::RemoveKeyboardCallback(int key, KeyboardAction action, const void* ownerPointer) {
    KeyboardInputsMap *  callbacks = GetKeyboardCallbackFromAction(action);
    if(callbacks->find(key) == callbacks->end()) {
        return;
    }

    callbacks->erase(key);
}



void InputManager::Reset() {
    callbacks_keyboard_pressed.clear();
    callbacks_keyboard_release.clear();
    callbacks_keyboard_repeat.clear();
}

void InputManager::QueueKeyboardInput(const KeyboardInput &input) {
    keyboardInputs.push(input);
}

void InputManager::ProcessInput() {
    // Processing REPEATING keys (Input polling)
    for(std::pair<int, std::map<const void* , Callback>> pair : callbacks_keyboard_repeat) {
        if(glfwGetKey(window, pair.first) == GLFW_PRESS) {
            for(std::pair<const void*, Callback> callback : pair.second) {
                callback.second();
            }
        }
    }

    // Processing PRESSED AND RELEASED keys (From window callback)
    while(!keyboardInputs.empty()) {
        const KeyboardInput& input = UnqueueKeyboardInput();
        auto callbacks = GetKeyboardCallbackFromAction(input.action);
        if(!callbacks || callbacks->find(input.key) == callbacks->end()) {
            continue;
        }

        //TODO Remove copy
        for(InputsPairKeyboard callback : (*callbacks)[input.key]) {
            callback.second();
        }
    }

    // Processing mouse inputs
    while(!MouseInputs.empty()) {
        const MouseInput& input = UnqueueMouseInput();
        auto callbacks = GetMouseCallbackFromAction(input.action);

        ;
        if(!callbacks || callbacks->find(input.key) == callbacks->end()) {
            continue;
        }

        //TODO remove copy
        for(InputsPairMouse callback : (*callbacks)[input.key]) {
            callback.second(input.position);
        }


    }

}

const KeyboardInput &InputManager::UnqueueKeyboardInput() {
    const KeyboardInput& input = keyboardInputs.front();
    keyboardInputs.pop();
    return input;
}

KeyboardInputsMap *InputManager::GetKeyboardCallbackFromAction(KeyboardAction action) {
    switch (action) {
        case KeyboardAction::VP_KEY_PRESSED:
            return &callbacks_keyboard_pressed;
        case KeyboardAction::VP_KEY_RELEASED:
            return &callbacks_keyboard_release;
        case KeyboardAction::VP_KEY_REPEAT:
            return &callbacks_keyboard_repeat;
        case KeyboardAction::VP_KEY_UNKNOWN:
            break;
    }
    return nullptr;
}

void InputManager::AddMouseCallback(int key, MouseAction action, const void* ownerPointer, const MouseCallback & callback) {

    auto callbacks = GetMouseCallbackFromAction(action);

    if(!callbacks) return;


    if(callbacks->find(key) == callbacks->end()) {
        callbacks->insert({});
    }
    if(action == MouseAction::VP_MOUSE_MOVE)
    {
        auto ez = (*callbacks)[0];
        ez.insert({ownerPointer, callback});
        (*callbacks)[0].insert({ownerPointer, callback});
    }
    (*callbacks)[key].insert({ownerPointer, callback});
}

MouseInputsMap *
InputManager::GetMouseCallbackFromAction(MouseAction action) {
    switch (action) {
        case MouseAction::VP_MOUSE_PRESSED:
            return &callbacks_mouse_pressed;
        case MouseAction::VP_MOUSE_RELEASED:
            return &callbacks_mouse_release;
        case MouseAction::VP_MOUSE_MOVE:
            return &callbacks_mouse_moved;
        default:
            break;
    }
    return nullptr;
}

void InputManager::RemoveMouseCallback(int key, MouseAction action, const void* ownerPointer) {
    auto callbacks = GetMouseCallbackFromAction(action);

    if(!callbacks) return;

    if(callbacks->find(key) == callbacks->end()) {
        return;
    }

    callbacks->erase(key);
}

const MouseInput &InputManager::UnqueueMouseInput() {
    const MouseInput& input = MouseInputs.front();
    MouseInputs.pop();
    return input;
}

void InputManager::QueueMouseInput(const MouseInput &input) {
    MouseInputs.push(input);
}

bool InputManager::IsKeyPressed(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool InputManager::IsMouseKeyPressed(int key) {
    return glfwGetMouseButton(window, key) == GLFW_PRESS;
}

InputManager::InputManager(GLFWwindow *context_window) : window(context_window) {

}
