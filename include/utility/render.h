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

void set_shader(unsigned int shader, const char* data_name, Matrix matrix) {
    // Send the matrices to the shader Note that OPENGL expects matrix to be column-majour order. 
    unsigned int object = glGetUniformLocation(shader, data_name);
    
    if (IS_VEC(matrix)) {
        return glUniform3fv(object, 1, (void*) matrix.data);
    }

    return glUniformMatrix4fv(object, 1, GL_TRUE, (void*) matrix.data);
}

void loadVertex(unsigned int* VAO, unsigned int* light_VAO, unsigned int* VBO) {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,  
        0.5f,  0.5f, -0.5f,  
        0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
        0.5f, -0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

        0.5f,  0.5f,  0.5f,  
        0.5f,  0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f,  0.5f,  
        0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
        0.5f,  0.5f, -0.5f,  
        0.5f,  0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, light_VAO);
    glBindVertexArray(*light_VAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return;
}

void render(GLFWwindow* window, unsigned int shaderProgram, unsigned int light_shader, unsigned int VAO, unsigned int light_VAO) {
    // Set the camera parameters
    Vector camera_pos = vec(3, 0.0f, 0.0f,  3.0f);
    Vector camera_front = vec(3, 0.0f, 0.0f, -1.0f);
    Vector camera_up = vec(3, 0.0f, 1.0f,  0.0f);
    Camera camera = init_camera(camera_pos, camera_front, camera_up, 2.5f);

    while (!glfwWindowShouldClose(window)) {
        // Update the camera speed
        update_camera_speed(&camera, FALSE);
        update_camera_front(camera, get_mouse_position());

        // Handle user input
        processInput(window, &camera);

        // Clean the window before rendering anything
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // Activate shader
        glUseProgram(shaderProgram);

        // Create the view matrix
        Matrix view = look_at(camera);

        // Create the projection matrix
        Matrix projection = perspective_matrix(get_scroll_position(), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);

        // Set the colour data for the light shader
        Vector object_vec = vec(3, 1.0f, 0.5f, 0.31f);
        Vector light_vec = vec(3, 1.0f, 1.0f, 1.0f);

        set_shader(shaderProgram, "objectColor", object_vec);
        set_shader(shaderProgram, "lightColor", light_vec);   

        // Send the matrices to the shader (The bool param is about column or row majour order, respectively GL_FALSE and GL_TRUE)
        set_shader(shaderProgram, "view", view);
        set_shader(shaderProgram, "projection", projection);

        Matrix model = create_identity_matrix(4);
        set_shader(shaderProgram, "model", model);

        // Render the cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Use the light shader
        glUseProgram(light_shader);

        // Set the data for the vertex shader
        set_shader(light_shader, "view", view);
        set_shader(light_shader, "projection", projection);   

        // Set the light source 
        Vector light_pos = vec(3, 1.2f, 1.0f, 2.0f);
        translate_mat(model, light_pos, &model);
        Vector scaling_vec = alloc_vector(0.2f, 3);
        scale_matrix(model, scaling_vec, &model); 
        deallocate_matrices(2, scaling_vec, light_pos);
        
        // Set the data for the vertex shader        
        set_shader(light_shader, "model", model);

        // Render the light cube
        glBindVertexArray(light_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Remove the used matrices
        deallocate_matrices(3, view, projection, model);
        
        // Swap buffers and poll IO events 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate the camera
    deallocate_camera(camera);

    return;
}

void terminate(unsigned int shaderProgram, unsigned int light_shader, unsigned int* VAO, unsigned int* light_VAO, unsigned int* VBO) {
    glDeleteVertexArrays(1, VAO);
    glDeleteVertexArrays(1, light_VAO);
    glDeleteBuffers(1, VBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(light_shader);
    glfwTerminate();
    return;
}

#endif //_RENDER_H_