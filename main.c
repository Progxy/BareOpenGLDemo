#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/glad/glad.h"
#include "./include/GLFW/glfw3.h"
#define _STDLIB_DEF_
#include "./include/utility/utils.h"
#include "./include/utility/parser.h"
#include "./include/utility/loader.h"
#include "./include/utility/input.h"
#include "./include/utility/render.h"

int main() {
    // Init the window and check the status of the operation
    GLFWwindow* window;
    if ((window = initWindow(800, 600, "Game")) == NULL) {
        return -1;
    }

    // Init the shaders and check the status of the operation
    unsigned int shaderProgram;
    if ((shaderProgram = initShaders("./include/shaders/vertex.hlsl", "./include/shaders/fragment.hlsl")) != INT32_MAX) {
        return -1;
    }

    // Load vertex
    unsigned int VBO, VAO;
    loadVertex(&VAO, &VBO);

    render(window, shaderProgram, VAO);

    terminate(shaderProgram, VAO, VBO);

    return 0;
}

