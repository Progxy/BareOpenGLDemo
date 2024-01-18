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

void set_matrix(unsigned int shader, const char* obj_name, float* obj_data, void (*uniform_mat)(GLint, GLsizei, GLboolean, const GLfloat*)) {
    unsigned int object = glGetUniformLocation(shader, obj_name);   
    return (*uniform_mat)(object, 1, GL_TRUE, obj_data);
}

void set_vec(unsigned int shader, const char* obj_name, float* obj_data, void (*uniform_vec)(GLint, GLsizei, const GLfloat*)) {
    unsigned int object = glGetUniformLocation(shader, obj_name);
    return (*uniform_vec)(object, 1, obj_data);
}

void set_value(unsigned int shader, const char* obj_name, float obj_data, void (*uniform_value)(GLint, GLfloat)) {
    unsigned int object = glGetUniformLocation(shader, obj_name);
    return (*uniform_value)(object, obj_data);
}

void loadVertex(unsigned int* VAO, unsigned int* light_VAO, unsigned int* VBO) {
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, light_VAO);
    glBindVertexArray(*light_VAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return;
}

void set_light_properties(unsigned int vertex_shader, Vector light_pos, Vector view_pos) {
    // Use the light shader
    glUseProgram(vertex_shader);

    // Set the view position
    set_vec(vertex_shader, "viewPos", view_pos.data, glUniform3fv);

    // Set the data for the material props
    Vector amb_diff_vec = vec(3, 1.0f, 0.5f, 0.31f);
    Vector specular_vec = vec(3, 0.5f, 0.5f, 0.5f);

    // Set material properties
    set_vec(vertex_shader, "material.ambient", amb_diff_vec.data, glUniform3fv);
    set_vec(vertex_shader, "material.diffuse", amb_diff_vec.data, glUniform3fv);
    set_vec(vertex_shader, "material.specular", specular_vec.data, glUniform3fv);
    set_value(vertex_shader, "material.shininess", 32.0f, glUniform1f);
    deallocate_matrices(2, amb_diff_vec, specular_vec);

    // Set the data for the light props
    Vector ambient_light = vec(3, 0.2f, 0.2f, 0.2f);
    Vector diffuse_light = vec(3, 0.5f, 0.5f, 0.5f);
    Vector specular_light = vec(3, 1.0f, 1.0f, 1.0f);

    // Set light properties
    set_vec(vertex_shader, "light.ambient",  ambient_light.data, glUniform3fv);
    set_vec(vertex_shader, "light.diffuse",  diffuse_light.data, glUniform3fv); // darken diffuse light a bit
    set_vec(vertex_shader, "light.specular", specular_light.data, glUniform3fv);
    set_vec(vertex_shader, "light.position", light_pos.data, glUniform3fv);   
    deallocate_matrices(3, ambient_light, diffuse_light, specular_light);

    return;
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

void render(GLFWwindow* window, unsigned int vertex_shader, unsigned int light_shader, unsigned int VAO, unsigned int light_VAO) {
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

        Vector light_pos = vec(3, 1.2f, 1.0f, 2.0f);
        set_light_properties(vertex_shader, light_pos, camera.camera_pos);

        // Create the frustum (view, projection and model matrices)
        Matrix view = look_at(camera);
        Matrix projection = perspective_matrix(get_scroll_position(), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
        Matrix model = create_identity_matrix(4);
        set_frustum(vertex_shader, view, projection, model);

        // Render the cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Set the light source 
        translate_mat(model, light_pos, &model);
        Vector scaling_vec = alloc_vector(0.2f, 3);
        scale_matrix(model, scaling_vec, &model); 
        deallocate_matrices(2, scaling_vec, light_pos);
        set_frustum(light_shader, view, projection, model);

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

void terminate(unsigned int vertex_shader, unsigned int light_shader, unsigned int* VAO, unsigned int* light_VAO, unsigned int* VBO) {
    glDeleteVertexArrays(1, VAO);
    glDeleteVertexArrays(1, light_VAO);
    glDeleteBuffers(1, VBO);
    glDeleteProgram(vertex_shader);
    glDeleteProgram(light_shader);
    glfwTerminate();
    return;
}

#endif //_RENDER_H_