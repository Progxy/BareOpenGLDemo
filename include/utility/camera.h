#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <stdlib.h>
#include "./matrix.h"
#include "./utils.h"

typedef struct Camera {
    Vector* camera_pos;
    Vector* camera_target;
    Vector* camera_up;
} Camera;

Camera init_camera(Vector* camera_pos, Vector* camera_target, Vector* camera_up) {
    Camera camera = {.camera_pos = camera_pos, .camera_target = camera_target, .camera_up = camera_up};
    return camera;
}

Matrix* look_at(Camera camera) {
    // Retrieve the Right, Direction and Up vectors
    Vector* camera_direction = sum_matrix(*(camera.camera_pos), *(camera.camera_target), 1);
    camera_direction -> isVec = TRUE;
    normalize_vector(camera_direction);  
    Vector* camera_right = cross_product(*(camera.camera_up), *camera_direction);
    normalize_vector(camera_right);
    Vector* camera_up = cross_product(*camera_direction, *camera_right);

    // Add them as rows of the first matrix
    Matrix* dir_matrix = create_identity_matrix(4);

    for (unsigned int i = 0; i < 3; ++i) {
        (dir_matrix -> data)[i] = (camera_right -> data)[i];
        (dir_matrix -> data)[i + 4] = (camera_up -> data)[i];
        (dir_matrix -> data)[i + 8] = (camera_direction -> data)[i];
    }

    // Create the position matrix
    Matrix* pos_matrix = create_identity_matrix(4);

    for (unsigned int i = 0; i < 3; ++i) {
        (pos_matrix -> data)[i * 4 + 3] = -((camera.camera_pos) -> data)[i];
    }

    Matrix* look_at_mat = dot_product_matrix(*dir_matrix, *pos_matrix);
    deallocate_matrices(2, dir_matrix, pos_matrix);
    deallocate_matrices(3, camera_direction, camera_right, camera_up);

    return look_at_mat;
}

#endif //_CAMERA_H_