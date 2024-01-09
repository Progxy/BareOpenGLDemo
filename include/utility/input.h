#ifndef _INPUT_H_
#define _INPUT_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"
#endif //_STDLIB_DEF_

#include "./matrix.h"
#include "./camera.h"

void processInput(GLFWwindow* window, Camera camera) {
    const float camera_speed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        Vector temp = alloc_vector(0.0f, 1);
        scalar_product_matrix(camera.camera_front, camera_speed, &temp);
        sum_matrices(2, &(camera.camera_pos), camera.camera_pos, temp);
        deallocate_matrices(1, temp);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        Vector temp = alloc_vector(0.0f, 1);
        scalar_product_matrix(camera.camera_front, -camera_speed, &temp);
        sum_matrices(2, &(camera.camera_pos), camera.camera_pos, temp);
        deallocate_matrices(1, temp);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Vector temp = cross_product(camera.camera_front, camera.camera_up);
        normalize_vector(temp, &temp);
        scalar_product_matrix(temp, -camera_speed, &temp);
        sum_matrices(2, &(camera.camera_pos), camera.camera_pos, temp);
        gc_dispose();
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        Vector temp = cross_product(camera.camera_front, camera.camera_up);
        normalize_vector(temp, &temp);
        scalar_product_matrix(temp, camera_speed, &temp);
        sum_matrices(2, &(camera.camera_pos), camera.camera_pos, temp);
        gc_dispose();
    } else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, TRUE);
        printf("INPUT:KEY_PRESS_ESCAPE: closing the window...\n");
    }
    return;
}

#endif //_INPUT_H_