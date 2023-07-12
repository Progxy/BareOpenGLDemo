#ifndef _LOADER_H_
#define _LOADER_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"
#include "./glad/glad.h"

#endif //_STDLIB_DEF_

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

GLFWwindow* initWindow(int width, int height, const char* title) {
    // Init GLFW
    glfwInit();

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
    
    // Load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("GLAD:LOADING_POINTERS:ERROR: Failed to initialize GLAD\n");
        return NULL;
    }
    
    return window;
}

unsigned int initShaders(const char* pathVertexShader, const char* pathFragmentShader) {
    char infoLog[512];
    const char* fragmentShaderData;
    const char* vertexShaderData;
    int status;

    // Load shaders 
    if ((vertexShaderData = readFile(pathVertexShader)) == NULL) {
        return INT32_MAX;
    }    
    
    if ((fragmentShaderData = readFile(pathFragmentShader)) == NULL) {
        return INT32_MAX;
    }

    // Retrieve the vertex shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderData, NULL);
    glCompileShader(vertexShader);

    // Check for shader compile errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
        return INT32_MAX;
    }

    // Retrieve the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderData, NULL);
    glCompileShader(fragmentShader);
    
    // Check for shader compile errors
    glGetProgramiv(fragmentShader, GL_LINK_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
        return INT32_MAX;
    }

    // Link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED: %s\n", infoLog);
        return INT32_MAX;
    }

    // Deallocate the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

// Whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    return;
}

#endif //_LOADER_H_