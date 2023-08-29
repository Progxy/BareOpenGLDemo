#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_

#include <stdlib.h>
#include "./matrix.h"
#include "./utils.h"

/* TODO: Define all the transformations matrix and perspective matrix and more... */

Matrix* scale_matrix(Vector scaling_vec) {
    Matrix* scaling_mat = create_identity_matrix(scaling_vec.rows);

    for (int i = 0; i < scaling_vec.rows; ++i) {
        MAT_INDEX(*scaling_mat, i, i) = VEC_INDEX(scaling_vec, i);
    }

    return scaling_mat;
}

Matrix* translate_matrix(Vector translation_vec) {
    Matrix* translation_mat = create_identity_matrix(translation_vec.rows);
    
    for (int i = 0; i < translation_vec.rows; ++i) {
        MAT_INDEX(*translation_mat, i, 3) = VEC_INDEX(translation_vec, i);
    }

    return translation_mat;
}

Matrix* rotation_x_matrix(float angle, int size) {
    Matrix* rotation_x_mat = create_identity_matrix(size);
    MAT_INDEX(*rotation_x_mat, 1, 1) = cosfr(deg_to_rad(angle));
    MAT_INDEX(*rotation_x_mat, 1, 2) = remove_neg_sign(-sinfr(deg_to_rad(angle)));
    MAT_INDEX(*rotation_x_mat, 2, 1) = sinfr(deg_to_rad(angle));
    MAT_INDEX(*rotation_x_mat, 2, 2) = cosfr(deg_to_rad(angle));

    return rotation_x_mat;
}

Matrix* rotation_y_matrix(float angle, int size) {
    Matrix* rotation_y_mat = create_identity_matrix(size);
    MAT_INDEX(*rotation_y_mat, 0, 0) = cosfr(deg_to_rad(angle));
    MAT_INDEX(*rotation_y_mat, 0, 2) = sinfr(deg_to_rad(angle));
    MAT_INDEX(*rotation_y_mat, 2, 0) = remove_neg_sign(-sinfr(deg_to_rad(angle)));
    MAT_INDEX(*rotation_y_mat, 2, 2) = cosfr(deg_to_rad(angle));

    return rotation_y_mat;
} 

Matrix* rotation_z_matrix(float angle, int size) {
    Matrix* rotation_z_mat = create_identity_matrix(size);
    MAT_INDEX(*rotation_z_mat, 0, 0) = cosfr(deg_to_rad(angle));
    MAT_INDEX(*rotation_z_mat, 0, 1) = remove_neg_sign(-sinfr(deg_to_rad(angle)));
    MAT_INDEX(*rotation_z_mat, 1, 0) = sinfr(deg_to_rad(angle));
    MAT_INDEX(*rotation_z_mat, 1, 1) = cosfr(deg_to_rad(angle));

    return rotation_z_mat;
}

// Function to create a perspective projection matrix
// http://www.songho.ca/opengl/gl_projectionmatrix.html
Matrix* perspective_matrix(float fov, float aspect, float near, float far) {
    assert(aspect != 0.0f);
    assert(far != near);
    
    Matrix* perspective_mat = alloc_matrix(0.0f, 4, 4, FALSE);
    
    // Calculate the tangent of half the vertical field of view and other parameters
    float tanHalfFov = tanf(deg_to_rad(fov) / 2.0f);
    float zRange = far - near;

    
    // Calculate the values for the projection matrix
    MAT_INDEX(*perspective_mat, 0, 0) =  (1.0f) / (aspect * tanHalfFov); 
    MAT_INDEX(*perspective_mat, 1, 1) = (1.0f) / (tanHalfFov);
    MAT_INDEX(*perspective_mat, 2, 2) = -(far + near) / zRange;
    MAT_INDEX(*perspective_mat, 2, 3) = ((-2.0f) * far * near) / zRange;
    MAT_INDEX(*perspective_mat, 3, 2) = -1.0f;

    return perspective_mat;
}
#endif //_TRANSFORMATION_H_