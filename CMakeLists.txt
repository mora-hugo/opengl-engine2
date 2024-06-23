cmake_minimum_required(VERSION 3.0.0)
project(opengl-imgui-cmake-template VERSION 0.1.0)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

link_directories(libs/glfw)

include_directories(libs/glad)
include_directories(libs/KHR)
include_directories(libs/imgui)
include_directories(libs/glfw)
include_directories(libs/stb_image)
add_subdirectory(libs/glm EXCLUDE_FROM_ALL)

include_directories(src/Public)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE SOURCES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Private/*.cpp)
add_executable(opengl-imgui-cmake-template ${SOURCES} src/Private/App.cpp src/Private/Window.cpp
        src/Private/Utils.cpp
        src/Private/Shader.cpp
        src/Private/Program.cpp
        src/Public/Model.h
        src/Public/Shaders.h
        src/Public/KeyboardInput.h
        src/Public/InputManager.h
        src/Public/Camera.h
        src/Public/Tests/Test.h
)

find_package(OpenGL REQUIRED)
target_link_libraries(opengl-imgui-cmake-template PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(opengl-imgui-cmake-template PRIVATE glfw3)
target_link_libraries(opengl-imgui-cmake-template PRIVATE glm)

file(COPY resources DESTINATION /)
