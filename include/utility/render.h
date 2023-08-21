#ifndef _RENDER_H_
#define _RENDER_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"
#include "./input.h"

#endif //_STDLIB_DEF_

#include "./matrix.h"

void loadVertex(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO) {
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
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

        // create transformations
        Matrix* transform_mat = create_identity_matrix(4);
        Vector* translation_vec = vec4(0.5f, -0.5f, 0.0f, 1.0f);
        Matrix* translation_mat = translate_matrix(*translation_vec);
        deallocate_matrix(translation_vec);
        Matrix* transform_result = dot_product_matrix(*translation_mat, *transform_mat);
        deallocate_matrix(transform_mat);
        deallocate_matrix(translation_mat);
        transform_mat = transform_result;
        Matrix* rotation_mat = rotation_x_matrix(90.0, 4);
        Matrix* rotation_result = dot_product_matrix(*rotation_mat, *transform_mat);
        deallocate_matrix(transform_mat);
        deallocate_matrix(rotation_mat);
        transform_mat = rotation_result;

        // get matrix's uniform location and set matrix
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform_mat -> data);
        

        // Use the shaders
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events 
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Remove the used transformation matrix
        deallocate_matrix(transform_mat);

    }
    return;
}

void terminate(unsigned int shaderProgram, unsigned int* VAO, unsigned int* VBO, unsigned int* EBO) {
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
    glDeleteBuffers(1, EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return;
}

#endif //_RENDER_H_