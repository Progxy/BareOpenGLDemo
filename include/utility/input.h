#ifndef _INPUT_H_
#define _INPUT_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"
#endif //_STDLIB_DEF_

#include "./utils.h"
#include "./matrix.h"
#include "./camera.h"

#define get_mouse_position() refresh_mouse_position(0.0f, 0.0f, TRUE)
#define get_scroll_position() refresh_scroll_position(0.0f, TRUE)   
#define reset_angles(yaw, pitch) refresh_mouse_position(yaw, pitch, TRUE)
#define GET_PRESSED_KEY(window, key) (glfwGetKey(window, key) == GLFW_PRESS)

float* refresh_mouse_position(float x_offset, float y_offset, unsigned char ret);

void processInput(GLFWwindow* window, Camera* camera) {
    glPolygonMode(GL_FRONT_AND_BACK, GET_PRESSED_KEY(window, GLFW_KEY_L) ? GL_LINE : GL_FILL); // Set to wireframe mode

    if (GET_PRESSED_KEY(window, GLFW_KEY_W)) {
        Vector temp = alloc_vector(0.0f, 1);
        scalar_product_matrix(camera -> camera_front, camera -> camera_speed, &temp);
        SUM_MATRICES(&(camera -> camera_pos), camera -> camera_pos, temp);
        DEALLOCATE_MATRICES(temp);
    } else if (GET_PRESSED_KEY(window, GLFW_KEY_S)) {
        Vector temp = alloc_vector(0.0f, 1);
        scalar_product_matrix(camera -> camera_front, -camera -> camera_speed, &temp);
        SUM_MATRICES(&(camera -> camera_pos), camera -> camera_pos, temp);
        DEALLOCATE_MATRICES(temp);
    } else if (GET_PRESSED_KEY(window, GLFW_KEY_A)) {
        Vector temp = cross_product(camera -> camera_front, camera -> camera_up);
        normalize_vector(temp, &temp);
        scalar_product_matrix(temp, -camera -> camera_speed, &temp);
        SUM_MATRICES(&(camera -> camera_pos), camera -> camera_pos, temp);
        gc_dispose();
    } else if (GET_PRESSED_KEY(window, GLFW_KEY_D)) {
        Vector temp = cross_product(camera -> camera_front, camera -> camera_up);
        normalize_vector(temp, &temp);
        scalar_product_matrix(temp, camera -> camera_speed, &temp);
        SUM_MATRICES(&(camera -> camera_pos), camera -> camera_pos, temp);
        gc_dispose();
    } else if (GET_PRESSED_KEY(window, GLFW_KEY_UP) || GET_PRESSED_KEY(window, GLFW_KEY_SPACE)) {
        Vector temp = VEC(0.0f, camera -> camera_speed, 0.0f);
        SUM_MATRICES(&(camera -> camera_pos), camera -> camera_pos, temp);
        DEALLOCATE_MATRICES(temp);
    } else if (GET_PRESSED_KEY(window, GLFW_KEY_DOWN)) {
        Vector temp = VEC(0.0f, -(camera -> camera_speed), 0.0f);
        SUM_MATRICES(&(camera -> camera_pos), camera -> camera_pos, temp);
        DEALLOCATE_MATRICES(temp);
    } else if (GET_PRESSED_KEY(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, TRUE);
        printf("INPUT:KEY_PRESS_ESCAPE: closing the window...\n");
    } else if (GET_PRESSED_KEY(window, GLFW_KEY_R)) {
        // Reset the angles
        reset_angles(-90.0f, 0.0f);
        // Reset the camera
        deallocate_camera(*camera);
        Vector camera_pos = VEC(0.0f, 0.0f,  3.0f);
        Vector camera_front = VEC(0.0f, 0.0f, -1.0f);
        Vector camera_up = VEC(0.0f, 1.0f,  0.0f);
        *camera = init_camera(camera_pos, camera_front, camera_up, 2.5f);
        // Reset the camera speed
        update_camera_speed(camera, TRUE);
        printf("INPUT:RESET_KEY_PRESS: resetting the camera...\n");
    }

    return;
}

float* refresh_mouse_position(float x_offset, float y_offset, unsigned char ret) {
    static float angles[2] = {-90.0f, 0.0f}; // Yaw, Pitch

    if (ret) {
        if (x_offset != 0.0f) {
            angles[0] = x_offset;
            angles[1] = y_offset;
            return NULL;
        }
        return angles;
    }

    // Update yaw and pitch and clip pitch to prevent vertical flipping
    angles[0] += x_offset;
    angles[1] += y_offset;
    angles[1] = CLIP(angles[1], -89.0f, 89.0f);

    return NULL;
}

void mouse_callback(GLFWwindow* /*window*/, double x_pos, double y_pos) {
    static float last_x = WIDTH / 2;
    static float last_y = HEIGHT / 2;

    refresh_mouse_position(SENSITIVITY * (x_pos - last_x), SENSITIVITY * (last_y - y_pos), FALSE); // reversed since y-coordinates range from bottom to top
    last_x = x_pos;
    last_y = y_pos;

    return;
}

float refresh_scroll_position(float offset, unsigned char ret) {
    static float fov = 45.0f;

    if (ret) {
        return fov;
    }

    // Update and clip the scroll position
    fov -= offset; // reversed since z-coordinates range from far to near
    fov = CLIP(fov, 1.0f, 45.0f);

    return 0.0f;
}

void scroll_callback(GLFWwindow* /*window*/, double /*x_offset*/, double y_offset) {
    refresh_scroll_position(y_offset, FALSE);
    return;
}

#endif //_INPUT_H_