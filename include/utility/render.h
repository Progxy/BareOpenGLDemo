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
#include "./camera.h"

void temploadVertex(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO) {
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return;
}

void loadVertex(unsigned int* VAO, unsigned int* VBO, unsigned int shaderProgram) {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Use the shaders
    glUseProgram(shaderProgram);

    return;
}

void render(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO) {
    Vector cube_positions[] = {
        vec(3,  0.0f,  0.0f,  0.0f),
        vec(3,  2.0f,  5.0f, -15.0f),
        vec(3, -1.5f, -2.2f, -2.5f),
        vec(3, -3.8f, -2.0f, -12.3f),
        vec(3,  2.4f, -0.4f, -3.5f),
        vec(3, -1.7f,  3.0f, -7.5f),
        vec(3,  1.3f, -2.0f, -2.5f),
        vec(3,  1.5f,  2.0f, -2.5f),
        vec(3,  1.5f,  0.2f, -1.5f),
        vec(3, -1.3f,  1.0f, -1.5f)
    };

    // Set the camera parameters
    Vector camera_pos = vec(3, 0.0f, 0.0f,  3.0f);
    Vector camera_front = vec(3, 0.0f, 0.0f, -1.0f);
    Vector camera_up = vec(3, 0.0f, 1.0f,  0.0f);
    Camera camera = init_camera(camera_pos, camera_front, camera_up, 2.5f);

    while (!glfwWindowShouldClose(window)) {
        // Update the camera speed
        update_camera_speed(&camera);

        update_camera_front(camera, get_mouse_position());

        // Handle user input
        processInput(window, camera);

        // Clean the window before rendering anything
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // Activate shader
        glUseProgram(shaderProgram);

        // Create the view matrix
        Matrix view = look_at(camera);

        // Create the projection matrix
        Matrix projection = perspective_matrix(get_scroll_position(), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);

        // Send the matrices to the shader (The bool param is about column or row majour order, respectively GL_FALSE and GL_TRUE)
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_TRUE, view.data);
        
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, projection.data);

        // Render container
        glBindVertexArray(VAO);

        // Draw cubes
        for (unsigned int i = 0; i < 10; ++i) {
            // Create the model matrix
            Matrix model = create_identity_matrix(4);
            translate_mat(model, cube_positions[i], &model);
            Vector model_vec = vec(3, 1.0f, 0.3f, 0.5f);
            float angle = 20.0f * i;
            rotate_matrix(model, deg_to_rad(angle), model_vec, &model);
            deallocate_matrices(1, model_vec);

            // Send the model matrix to the vertex shader
            unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_TRUE, model.data);

            glDrawArrays(GL_TRIANGLES, 0, 36);
            
            // Remove the used matrix
            deallocate_matrices(1, model);
        }

        // Swap buffers and poll IO events 
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Remove the used matrices
        deallocate_matrices(2, view, projection);
    }

    // Deallocate the camera
    deallocate_camera(camera);

    return;
}

void terminate(unsigned int shaderProgram, unsigned int* VAO, unsigned int* VBO) {
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return;
}

#endif //_RENDER_H_