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
#define _USE_IMAGE_LIBRARY_
#include "./libs/image_io.h"

int main() {
    // Init the window and check the status of the operation
    GLFWwindow* window;
    if ((window = init_window(WIDTH, HEIGHT, "Game")) == NULL) {
        return -1;
    }

    DEBUG_INFO("Loaded window");

    // Init the shaders and check the status of the operation
    unsigned int vertex_shader;
    if ((vertex_shader = init_shaders((const char*) "./include/shaders/vertex.glsl", (const char*) "./include/shaders/fragment.glsl")) == INT32_MAX) {
        return -1;
    }

    // Init the shaders and check the status of the operation
    unsigned int light_shader;
    if ((light_shader = init_shaders((const char*) "./include/shaders/light_vertex.glsl", (const char*) "./include/shaders/light_shader.glsl")) == INT32_MAX) {
        return -1;
    }

    printf("DEBUG_INFO: Loaded shader program, shader: %u, light_shader: %u\n", vertex_shader, light_shader);

    // Load vertex
    unsigned int VBO, VAO, light_VAO;
    loadVertex(&VAO, &light_VAO, &VBO);

    DEBUG_INFO("Rendering...");        

    render(window, vertex_shader, light_shader, VAO, light_VAO);

    DEBUG_INFO("terminating the program...");

    terminate(vertex_shader, light_shader, &VAO, &light_VAO, &VBO);

    Image image = read_file("./assets");

    return 0;
}

