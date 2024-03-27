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
#include "./model.h"

void set_matrix(unsigned int shader, const char* obj_name, float* obj_data, void (*uniform_mat)(GLint, GLsizei, GLboolean, const GLfloat*)) {
    unsigned int object = glGetUniformLocation(shader, obj_name);   
    return (*uniform_mat)(object, 1, GL_TRUE, obj_data);
}

void set_vec(unsigned int shader, const char* obj_name, float* obj_data, void (*uniform_vec)(GLint, GLsizei, const GLfloat*)) {
    unsigned int object = glGetUniformLocation(shader, obj_name);
    return (*uniform_vec)(object, 1, obj_data);
}

void set_float(unsigned int shader, const char* obj_name, float obj_data, void (*uniform_value)(GLint, GLfloat)) {
    unsigned int object = glGetUniformLocation(shader, obj_name);
    return (*uniform_value)(object, obj_data);
}

void set_frustum(unsigned int shader, Matrix view, Matrix projection, Matrix model) {
    // Use the light shader
    glUseProgram(shader);

    // Set the data for the vertex shader
    set_matrix(shader, "view", view.data, glUniformMatrix4fv);
    set_matrix(shader, "projection", projection.data, glUniformMatrix4fv); 
    set_matrix(shader, "model", model.data, glUniformMatrix4fv);

    return;
}

void render(GLFWwindow* window, unsigned int vertex_shader) {
    // Set the camera parameters
    Vector camera_pos = vec(3, 0.0f, 0.0f,  3.0f);
    Vector camera_front = vec(3, 0.0f, 0.0f, -1.0f);
    Vector camera_up = vec(3, 0.0f, 1.0f,  0.0f);
    Camera camera = init_camera(camera_pos, camera_front, camera_up, 2.5f);
    Model element = load_model("/home/Emanuele/Informatica/OpenGL/assets/survival_guitar_backpack/");
    glEnable(GL_DEPTH_TEST); // configure global opengl state

    while (!glfwWindowShouldClose(window)) {
        // Update the camera speed
        update_camera_speed(&camera, FALSE);
        update_camera_front(camera, get_mouse_position());

        // Handle user input
        processInput(window, &camera);

        // Clean the window before rendering anything
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // Create the frustum (view, projection and model matrices)
        Matrix view = look_at(camera);
        Matrix projection = perspective_matrix(get_scroll_position(), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
        Matrix model = create_identity_matrix(4);
        set_frustum(vertex_shader, view, projection, model);
        deallocate_matrices(3, view, projection, model);   

        // Render the cubes
        draw_model(vertex_shader, element);
        
        // Swap buffers and poll IO events 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate the camera
    deallocate_camera(camera);

    // Deallocate model
    deallocate_model(element);

    return;
}

void terminate(unsigned int vertex_shader) {
    glDeleteProgram(vertex_shader);
    glfwTerminate();
    return;
}

#endif //_RENDER_H_