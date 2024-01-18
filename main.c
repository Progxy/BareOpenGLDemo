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
    if ((window = initWindow(WIDTH, HEIGHT, "Game")) == NULL) {
        return -1;
    }

    DEBUG_INFO("Loaded window");

    // Init the shaders and check the status of the operation
    unsigned int shaderProgram;
    if ((shaderProgram = initShaders((const char*) "./include/shaders/vertex.hlsl", (const char*) "./include/shaders/fragment.hlsl")) == INT32_MAX) {
        return -1;
    }

    // Init the shaders and check the status of the operation
    unsigned int light_shader;
    if ((light_shader = initShaders((const char*) "./include/shaders/light_vertex.hlsl", (const char*) "./include/shaders/light_shader.hlsl")) == INT32_MAX) {
        return -1;
    }

    printf("DEBUG_INFO: Loaded shader program, shader: %u, light_shader: %u\n", shaderProgram, light_shader);

    // Load vertex
    unsigned int VBO, VAO, light_VAO;
    loadVertex(&VAO, &light_VAO, &VBO);

    DEBUG_INFO("Rendering...");        

    render(window, shaderProgram, light_shader, VAO, light_VAO);

    DEBUG_INFO("terminating the program...");

    terminate(shaderProgram, light_shader, &VAO, &light_VAO, &VBO);

    return 0;
}

