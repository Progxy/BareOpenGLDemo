#ifndef _RENDER_H_
#define _RENDER_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"
#include "./input.h"

#endif //_STDLIB_DEF_

#include "./transformation.h"

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

        // Use the shaders
        glUseProgram(shaderProgram);

        // Create the model matrix
        Matrix* model = create_identity_matrix(4);
        Matrix* temp_garbage = model;
        Vector* model_vec = vec3(1.0f, 0.0f, 0.0f);
        model = rotate_matrix(model, -55.0f, model_vec);
        deallocate_matrices(2, temp_garbage, model_vec);

        // Create the view matrix
        Vector* translation_vec = vec4(0.0f, 0.0f, -3.0f, 1.0f);
        Matrix* view = translate_matrix(*translation_vec);
        deallocate_matrix(translation_vec); 

        // Create the projection matrix
        Matrix* projection = perspective_matrix(45.0f, (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);

        // Send the matrices to the shader
        print_matrix(*model, "Model Matrix");
        print_matrix(*view, "View Matrix");
        print_matrix(*projection, "Projection Matrix");
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model -> data);
        
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view -> data);
        
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection -> data);

        // Render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events 
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Remove the used matrix
        deallocate_matrix(model);
        deallocate_matrix(view);
        deallocate_matrix(projection);
    
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