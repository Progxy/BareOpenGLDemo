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

void set_frustum(unsigned int shader, Camera camera) {
    Matrix view = look_at(camera);
    Matrix projection = perspective_matrix(get_scroll_position(), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
    Matrix camera_matrix = create_identity_matrix(4);
    DOT_PRODUCT_MATRIX(&camera_matrix, projection, view);
    set_matrix(shader, "camera_matrix", camera_matrix.data, glUniformMatrix4fv);
    DEALLOCATE_MATRICES(view, projection, camera_matrix);   
    return;
}

void render(GLFWwindow* window, unsigned int vertex_shader) {
    // Set the camera parameters
    Vector camera_pos = VEC(0.0f, 0.0f,  3.0f);
    Vector camera_front = VEC(0.0f, 0.0f, -1.0f);
    Vector camera_up = VEC(0.0f, 1.0f,  0.0f);
    Camera camera = init_camera(camera_pos, camera_front, camera_up, 2.5f);
    Model* object_model = load_model("/home/Emanuele/Informatica/OpenGL/assets/survival_guitar_backpack/");
    if (object_model == NULL) return;

	Vector light_color = alloc_vector(1.0f, 4);
    set_vec(vertex_shader, "light_color", light_color.data, glUniform4fv);
    DEALLOCATE_MATRICES(light_color);

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
        set_frustum(vertex_shader, camera);

        // Render the cubes
        draw_model(vertex_shader, object_model, &camera);
        
        // Swap buffers and poll IO events 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate the camera
    deallocate_camera(camera);

    // Deallocate model
    deallocate_model(object_model);

    return;
}

void terminate(unsigned int vertex_shader) {
    glDeleteProgram(vertex_shader);
    glfwTerminate();
    return;
}

#endif //_RENDER_H_