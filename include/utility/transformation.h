#ifndef _TRANSFORMATION_H_
#define _TRANSFORMATION_H_

#include <stdlib.h>
#include "./matrix.h"
#include "./utils.h"

void scale_matrix(Matrix scaling_mat, Vector scaling_vec, Matrix* dest) {
    unsigned int vec_size = VEC_SIZE(scaling_vec);
    assert(vec_size >= 3);

    Matrix temp = alloc_matrix(0.0f, 1, 1);
    copy_matrix(scaling_mat, &temp);

    for (unsigned int row = 0; row < scaling_mat.rows; ++row) {
        for (unsigned int col = 0; col < scaling_mat.cols; ++col) {
            MAT_INDEX(temp, row, col) = MAT_INDEX(scaling_mat, row, col) * ((vec_size == col) ? 1.0f : VEC_INDEX(scaling_vec, col));
        }
    }

    copy_matrix(temp, dest);
    deallocate_matrices(1, temp);

    return;
}

void rotation_x_matrix(float angle, int size, Matrix* dest) {
    Matrix rotation_x_mat = create_identity_matrix(size);

    MAT_INDEX(rotation_x_mat, 1, 1) = cosf(deg_to_rad(angle));
    MAT_INDEX(rotation_x_mat, 1, 2) = (-sinf(deg_to_rad(angle)));
    MAT_INDEX(rotation_x_mat, 2, 1) = sinf(deg_to_rad(angle));
    MAT_INDEX(rotation_x_mat, 2, 2) = cosf(deg_to_rad(angle));

    // Copy the rotation_mat
    copy_matrix(rotation_x_mat, dest);

    // Deallocate unused matrix
    deallocate_matrices(1, rotation_x_mat);

    return;
}

void rotation_y_matrix(float angle, int size, Matrix* dest) {
    Matrix rotation_y_mat = create_identity_matrix(size);

    MAT_INDEX(rotation_y_mat, 0, 0) = cosf(deg_to_rad(angle));
    MAT_INDEX(rotation_y_mat, 0, 2) = sinf(deg_to_rad(angle));
    MAT_INDEX(rotation_y_mat, 2, 0) = (-sinf(deg_to_rad(angle)));
    MAT_INDEX(rotation_y_mat, 2, 2) = cosf(deg_to_rad(angle));

    // Copy the rotation_mat
    copy_matrix(rotation_y_mat, dest);

    // Deallocate unused matrix
    deallocate_matrices(1, rotation_y_mat);

    return;
} 

void rotation_z_matrix(float angle, int size, Matrix* dest) {
    Matrix rotation_z_mat = create_identity_matrix(size);

    MAT_INDEX(rotation_z_mat, 0, 0) = cosf(deg_to_rad(angle));
    MAT_INDEX(rotation_z_mat, 0, 1) = (-sinf(deg_to_rad(angle)));
    MAT_INDEX(rotation_z_mat, 1, 0) = sinf(deg_to_rad(angle));
    MAT_INDEX(rotation_z_mat, 1, 1) = cosf(deg_to_rad(angle));

    // Copy the rotation_mat
    copy_matrix(rotation_z_mat, dest);

    // Deallocate unused matrix
    deallocate_matrices(1, rotation_z_mat);

    return;
}

// Function to create a perspective projection matrix
// http://www.songho.ca/opengl/gl_projectionmatrix.html
Matrix perspective_matrix(float fov, float aspect, float near, float far) {
    assert(aspect != 0.0f);
    assert(far != near);
    
    Matrix perspective_mat = alloc_matrix(0.0f, 4, 4);
    
    // Calculate the tangent of half the vertical field of view and other parameters
    float tanHalfFov = tanf(deg_to_rad(fov) / 2.0f);
    float zRange = far - near;
    
    // Calculate the values for the projection matrix
    MAT_INDEX(perspective_mat, 0, 0) =  (1.0f) / (aspect * tanHalfFov); 
    MAT_INDEX(perspective_mat, 1, 1) = (1.0f) / (tanHalfFov);
    MAT_INDEX(perspective_mat, 2, 2) = -(far + near) / zRange;
    MAT_INDEX(perspective_mat, 2, 3) = ((-2.0f) * far * near) / zRange;
    MAT_INDEX(perspective_mat, 3, 2) = -1.0f;

    return perspective_mat;
}

void rotate_matrix(Matrix src, float angle, Vector vec, Matrix* dest) {
    unsigned int vec_size = VEC_SIZE(vec);
    assert(vec_size == 3);

    float a = angle;
    float c = cosf(a);
    float s = sinf(a);

    Vector axis = alloc_vector(0.0f, 1);
    normalize_vector(vec, &axis);
    Vector temp = alloc_vector(0.0f, 1);
    scalar_product_matrix(axis, (1.0f - c), &temp);

    Matrix rotate = alloc_matrix(0.0f, 4, 4);
    MAT_INDEX(rotate, 0, 0) = c + VEC_INDEX(temp, 0) * VEC_INDEX(axis, 0);
    MAT_INDEX(rotate, 1, 0) = 0 + VEC_INDEX(temp, 0) * VEC_INDEX(axis, 1) + s * VEC_INDEX(axis, 2);
    MAT_INDEX(rotate, 2, 0) = 0 + VEC_INDEX(temp, 0) * VEC_INDEX(axis, 2) - s * VEC_INDEX(axis, 1);

    MAT_INDEX(rotate, 0, 1) = 0 + VEC_INDEX(temp, 1) * VEC_INDEX(axis, 0) - s * VEC_INDEX(axis, 2);
    MAT_INDEX(rotate, 1, 1) = c + VEC_INDEX(temp, 1) * VEC_INDEX(axis, 1);
    MAT_INDEX(rotate, 2, 1) = 0 + VEC_INDEX(temp, 1) * VEC_INDEX(axis, 2) + s * VEC_INDEX(axis, 0);

    MAT_INDEX(rotate, 0, 2) = 0 + VEC_INDEX(temp, 2) * VEC_INDEX(axis, 0) + s * VEC_INDEX(axis, 1);
    MAT_INDEX(rotate, 1, 2) = 0 + VEC_INDEX(temp, 2) * VEC_INDEX(axis, 1) - s * VEC_INDEX(axis, 0);
    MAT_INDEX(rotate, 2, 2) = c + VEC_INDEX(temp, 2) * VEC_INDEX(axis, 2);
    
    // Deallocate unused matrices
    deallocate_matrices(2, axis, temp);


    Matrix result = alloc_matrix(0.0f, 4, 4);
    Vector mats[3] = {};

    for (unsigned int i = 0; i < 3; ++i) {
        mats[i] = get_col(src, i);
    }

    Vector mat0 = alloc_vector(0.0f, 1);
    Vector mat1 = alloc_vector(0.0f, 1);
    Vector mat2 = alloc_vector(0.0f, 1);

    // result[0] = mat0 * MAT_INDEX(rotate, 0, 0) + mat1 * MAT_INDEX(rotate, 1, 0) + mat2 * MAT_INDEX(rotate, 2, 0);
    scalar_product_matrix(mats[0], MAT_INDEX(rotate, 0, 0), &mat0);
    scalar_product_matrix(mats[1], MAT_INDEX(rotate, 1, 0), &mat1);
    scalar_product_matrix(mats[2], MAT_INDEX(rotate, 2, 0), &mat2);
    sum_matrices(3, &mat0, mat0, mat1, mat2);
    copy_vector_to_matrix_col(mat0, result, 0);
    
    // result[1] = mat0 * MAT_INDEX(*rotate, 0, 1) + mat1 * MAT_INDEX(*rotate, 1, 1) + mat2 * MAT_INDEX(*rotate, 2, 1);
    scalar_product_matrix(mats[0], MAT_INDEX(rotate, 0, 1), &mat0);
    scalar_product_matrix(mats[1], MAT_INDEX(rotate, 1, 1), &mat1);
    scalar_product_matrix(mats[2], MAT_INDEX(rotate, 2, 1), &mat2);
    sum_matrices(3, &mat0, mat0, mat1, mat2);
    copy_vector_to_matrix_col(mat0, result, 1);

    // result[2] = mat0 * MAT_INDEX(*rotate, 0, 2) + mat1 * MAT_INDEX(*rotate, 1, 2) + mat2 * MAT_INDEX(*rotate, 2, 2);
    scalar_product_matrix(mats[0], MAT_INDEX(rotate, 0, 2), &mat0);
    scalar_product_matrix(mats[1], MAT_INDEX(rotate, 1, 2), &mat1);
    scalar_product_matrix(mats[2], MAT_INDEX(rotate, 2, 2), &mat2);
    sum_matrices(3, &mat0, mat0, mat1, mat2);
    copy_vector_to_matrix_col(mat0, result, 2);
    deallocate_matrices(2, mat1, mat2);

    // result[3] = get_col(mat, 3);
    copy_vector_to_matrix_col(get_col(src, 3), result, 3);
    
    // Deallocate all the unused resources
    deallocate_matrices(2, mat0, rotate);
    gc_dispose();

    // Copy the result matrix
    copy_matrix(result, dest);

    // Deallocate result matrix
    deallocate_matrices(1, result);

    return;
}

void translate_mat(Matrix src, Vector vec, Matrix* dest) {
    Matrix temp = alloc_matrix(0.0f, 1, 1);
    copy_matrix(src, &temp);

    Vector mat0 = alloc_vector(0.0f, 1);
    Vector mat1 = alloc_vector(0.0f, 1);
    Vector mat2 = alloc_vector(0.0f, 1);

    // result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
    scalar_product_matrix(get_col(src, 0), VEC_INDEX(vec, 0), &mat0);
    scalar_product_matrix(get_col(src, 1), VEC_INDEX(vec, 1), &mat1);
    scalar_product_matrix(get_col(src, 2), VEC_INDEX(vec, 2), &mat2);
    sum_matrices(4, &mat0, mat0, mat1, mat2, get_col(src, 3));
    copy_vector_to_matrix_col(mat0, temp, 3);

    // Deallocate all the unused resources
    deallocate_matrices(3, mat0, mat1, mat2);
    gc_dispose();

    // Copy the result
    copy_matrix(temp, dest);

    // Deallocate temp matrix
    deallocate_matrices(1, temp);

    return;
}

#endif //_TRANSFORMATION_H_