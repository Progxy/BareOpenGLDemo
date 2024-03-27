#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./include/glad/glad.h"
#include "./include/GLFW/glfw3.h"
#define _STDLIB_DEF_
#define _USE_IMAGE_LIBRARY_
#include "./libs/image_io.h"
#include "./include/utility/utils.h"
#include "./include/utility/parser.h"
#include "./include/utility/loader.h"
#include "./include/utility/input.h"
#include "./include/utility/render.h"
#include "./include/utility/texture.h"
#include "./include/utility/model.h"

int main() {
    // Init the window and check the status of the operation
    GLFWwindow* window;
    if ((window = init_window(WIDTH, HEIGHT, "Game")) == NULL) {
        return -1;
    }

    debug_info("Loaded window");

    // Init the shaders and check the status of the operation
    unsigned int vertex_shader;
    if ((vertex_shader = init_shaders((const char*) "./include/shaders/vertex.glsl", (const char*) "./include/shaders/fragment.glsl")) == INT32_MAX) {
        return -1;
    }

    debug_info("Loaded shader program, shader: %u\n", vertex_shader);

    debug_info("Rendering...");        

    render(window, vertex_shader);

    debug_info("terminating the program...");

    terminate(vertex_shader);

    return 0;
}

