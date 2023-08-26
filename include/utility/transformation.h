#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_

#include <stdlib.h>
#include "./matrix.h"

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
    MAT_INDEX(*rotation_x_mat, 1, 1) = cosfr(DEGREE_TO_RADIANS(angle));
    MAT_INDEX(*rotation_x_mat, 1, 2) = remove_neg_sign(-sinfr(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_x_mat, 2, 1) = sinfr(DEGREE_TO_RADIANS(angle));
    MAT_INDEX(*rotation_x_mat, 2, 2) = cosfr(DEGREE_TO_RADIANS(angle));

    return rotation_x_mat;
}

Matrix* rotation_y_matrix(float angle, int size) {
    Matrix* rotation_y_mat = create_identity_matrix(size);
    MAT_INDEX(*rotation_y_mat, 0, 0) = cosfr(DEGREE_TO_RADIANS(angle));
    MAT_INDEX(*rotation_y_mat, 0, 2) = sinfr(DEGREE_TO_RADIANS(angle));
    MAT_INDEX(*rotation_y_mat, 2, 0) = remove_neg_sign(-sinfr(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_y_mat, 2, 2) = cosfr(DEGREE_TO_RADIANS(angle));

    return rotation_y_mat;
} 

Matrix* rotation_z_matrix(float angle, int size) {
    Matrix* rotation_z_mat = create_identity_matrix(size);
    MAT_INDEX(*rotation_z_mat, 0, 0) = cosfr(DEGREE_TO_RADIANS(angle));
    MAT_INDEX(*rotation_z_mat, 0, 1) = remove_neg_sign(-sinfr(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_z_mat, 1, 0) = sinfr(DEGREE_TO_RADIANS(angle));
    MAT_INDEX(*rotation_z_mat, 1, 1) = cosfr(DEGREE_TO_RADIANS(angle));

    return rotation_z_mat;
}

// Function to create a perspective projection matrix
// http://www.songho.ca/opengl/gl_projectionmatrix.html
Matrix* perspective_matrix(float fov, float aspect, float near, float far) {
    Matrix* perspective_mat = alloc_matrix(0.0f, 4, 4, FALSE);
    
    // Calculate the tangent of half the vertical field of view and other parameters
    float tanHalfFov = tanfr(DEGREE_TO_RADIANS(fov / 2.0f));
    float halfHeight = near * tanHalfFov;   
    float halfWidth = halfHeight * aspect;
    float left =  halfWidth;
    float right = -halfWidth;
    float top =  halfHeight;
    float bottom = -halfHeight;
    float zRange = far - near;

    
    // Calculate the values for the projection matrix
    MAT_INDEX(*perspective_mat, 0, 0) =  (2.0f * near) / (right + left); 
    MAT_INDEX(*perspective_mat, 0, 2) =  (right + left) / (right - left);
    MAT_INDEX(*perspective_mat, 1, 1) = (2.0f * near) / (top - bottom);
    MAT_INDEX(*perspective_mat, 1, 2) = (top + bottom) / (top - bottom);
    MAT_INDEX(*perspective_mat, 2, 2) = ((-1.0f) * (near + far)) / zRange;
    MAT_INDEX(*perspective_mat, 2, 3) = -(2.0f * far * near) / zRange;
    MAT_INDEX(*perspective_mat, 3, 2) = -1.0f;
    
    return perspective_mat;
}
#endif //_TRANSFORMATION_H_