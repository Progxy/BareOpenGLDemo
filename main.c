#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/glad/glad.h"
#include "./include/GLFW/glfw3.h"
#include "./include/utility/utils.h"
#include "./include/utility/loader.h"
#include "./include/utility/render.h"

int main(void) {
    // Init the window and check the status of the operation
    GLFWwindow* window;
    if ((window = init_window(WIDTH, HEIGHT, "Game")) == NULL) {
        return -1;
    }

    debug_info("Loaded window\n");

    // Init the shaders and check the status of the operation
    unsigned int vertex_shader;
    if ((vertex_shader = init_shaders((const char*) "./include/shaders/vertex.glsl", (const char*) "./include/shaders/fragment.glsl")) == INT32_MAX) {
        return -1;
    }

    debug_info("Loaded shader program\n");

    debug_info("Rendering...\n");

    render(window, vertex_shader);

    debug_info("terminating the program...\n");

    terminate(vertex_shader);

    return 0;
}
