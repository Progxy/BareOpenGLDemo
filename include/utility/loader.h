#ifndef _LOADER_H_
#define _LOADER_H_

#include "./parser.h"
#include "./input.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* init_window(int width, int height, const char* title) {
    // Init GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        printf("GLFW:CREATE_WINDOW:ERROR: Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }

    // Set the context and the callback on resize
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Capture the mouse on window enter
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set cursor callback
    glfwSetCursorPosCallback(window, mouse_callback);

    // Set scroll callback
    glfwSetScrollCallback(window, scroll_callback);

    // Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("GLAD:LOADING_POINTERS:ERROR: Failed to initialize GLAD\n");
        return NULL;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    return window;
}

unsigned int init_shaders(const char* path_vertex_shader, const char* path_fragment_shader) {
    char infoLog[512];
    const char* fragment_shader_data;
    const char* vertex_shader_data;
    int status;

    // Load shaders
    if ((vertex_shader_data = read_file(path_vertex_shader)) == NULL) {
        return INT32_MAX;
    }

    if ((fragment_shader_data = read_file(path_fragment_shader)) == NULL) {
        return INT32_MAX;
    }

    // Retrieve the vertex shaders
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_data, NULL);
    glCompileShader(vertex_shader);

    // Check for shader compile errors
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
        return INT32_MAX;
    }

    // Retrieve the fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_data, NULL);
    glCompileShader(fragment_shader);

    // Check for shader compile errors
    glGetProgramiv(fragment_shader, GL_LINK_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
        return INT32_MAX;
    }

    // Link shaders
    unsigned int vertex_shader_program = glCreateProgram();
    glAttachShader(vertex_shader_program, vertex_shader);
    glAttachShader(vertex_shader_program, fragment_shader);
    glLinkProgram(vertex_shader_program);

    // Check for linking errors
    glGetProgramiv(vertex_shader_program, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(vertex_shader_program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED: %s\n", infoLog);
        return INT32_MAX;
    }

    // Deallocate the shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return vertex_shader_program;
}

// Whenever the window size changed (by OS or user resize) this callback function executes
/// @param: window is not used
void framebuffer_size_callback(UNUSED GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    return;
}

#endif //_LOADER_H_
