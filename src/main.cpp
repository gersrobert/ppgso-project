#include <iostream>
#include <map>

#include <ppgso/ppgso.h>
#include <src/objects/world/water.h>
#include "scene_window.h"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "[OpenGL Error](" << type << ") " << message << std::endl;
}

int main() {

    // Initialize our window
    SceneWindow window(WIDTH, HEIGHT);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
