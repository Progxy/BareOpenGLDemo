#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <stdlib.h>
#include "./matrix.h"
#include "./utils.h"

typedef struct Camera {
    Vector camera_pos;
    Vector camera_front;
    Vector camera_up;
    float camera_speed;
} Camera;

Camera init_camera(Vector camera_pos, Vector camera_front, Vector camera_up, float camera_speed) {
    Camera camera = {.camera_pos = camera_pos, .camera_front = camera_front, .camera_up = camera_up, .camera_speed = camera_speed};
    return camera;
}

void update_camera_front(Camera camera, float* mouse_pos) {
    float yaw = mouse_pos[0];
    float pitch = mouse_pos[1];
    Vector direction = VEC(cosf(deg_to_rad(yaw)) * cosf(deg_to_rad(pitch)), sinf(deg_to_rad(pitch)), sinf(deg_to_rad(yaw)) * cosf(deg_to_rad(pitch)));
    normalize_vector(direction, &(camera.camera_front));
    DEALLOCATE_MATRICES(direction);
    return;
}

void update_camera_speed(Camera* camera, unsigned char reset) {
    static float last_frame = 0.0f;
    static float delta_time = 1.0f;

    if (reset) {
        last_frame = 0.0f;
        delta_time = 1.0f;
    }

    float current_frame = glfwGetTime();
    camera -> camera_speed /= delta_time; 
    delta_time = current_frame - last_frame;
    camera -> camera_speed *= delta_time;
    last_frame = current_frame;
    return;
}

Matrix look_at(Camera camera) {
    // Retrieve the Right, Direction and Up vectors
    Vector camera_target = alloc_vector(0.0f, 1);
    SUM_MATRICES(&camera_target, camera.camera_front, camera.camera_pos);
    Vector camera_direction = alloc_vector(0.0f, 1);
    SUM_MATRICES(&camera_direction, camera.camera_pos, negate(camera_target));
    normalize_vector(camera_direction, &camera_direction);  
    Vector camera_right = cross_product(camera.camera_up, camera_direction);
    normalize_vector(camera_right, &camera_right);
    Vector camera_up = cross_product(camera_direction, camera_right);

    // Add them as rows of the first matrix
    Matrix dir_matrix = create_identity_matrix(4);

    for (unsigned int i = 0; i < 3; ++i) {
        MAT_INDEX(dir_matrix, 0, i) = VEC_INDEX(camera_right, i);
        MAT_INDEX(dir_matrix, 1, i) = VEC_INDEX(camera_up, i);
        MAT_INDEX(dir_matrix, 2, i) = VEC_INDEX(camera_direction, i);
    }

    // Create the position matrix
    Matrix pos_matrix = create_identity_matrix(4);

    for (unsigned int i = 0; i < 3; ++i) {
        MAT_INDEX(pos_matrix, i, 3) = -VEC_INDEX(camera.camera_pos, i);
    }

    Matrix look_at_mat = alloc_matrix(0.0f, 1, 1);
    DOT_PRODUCT_MATRIX(&look_at_mat, dir_matrix, pos_matrix);
    DEALLOCATE_MATRICES(dir_matrix, pos_matrix, camera_direction, camera_target);
    gc_dispose();

    return look_at_mat;
}

void deallocate_camera(Camera camera) {
    DEALLOCATE_MATRICES(camera.camera_pos, camera.camera_front, camera.camera_up); 
    return;
}

#endif //_CAMERA_H_