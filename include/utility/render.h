#ifndef _RENDER_H_
#define _RENDER_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"
#include "./input.h"
#endif //_STDLIB_DEF_

void loadVertex(unsigned int* VAO, unsigned int* VBO) {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom-left  
        0.5f, -0.5f, 0.0f, // bottom-right 
        -0.5f, 0.5f, 0.0f,  // top-left   
        0.5f,  0.5f, 0.0f  // top-right   
    }; 

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    return;
}

void render(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO) {
    while (!glfwWindowShouldClose(window)) {
        // Handle user input
        processInput(window);

        // Clean the window before rendering anything
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shaders
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, 4);

        // Swap buffers and poll IO events 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return;
}

void terminate(unsigned int shaderProgram, unsigned int VAO, unsigned int VBO) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return;
}

#endif //_RENDER_H_