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

Matrix* rotate_matrix(Matrix* mat, float angle, Vector* vec) {
    assert((vec -> rows) == 3);

    float a = angle;
    float c = cosf(a);
    float s = sinf(a);

    Vector* axis = duplicate_vector(vec);
    normalize_vector(axis);
    Vector* temp = duplicate_vector(axis);
    scalar_product_vector(*temp, (1.0f - c));

    Matrix* rotate = alloc_matrix(0.0f, 4, 4, FALSE);
    MAT_INDEX(*rotate, 0, 0) = c + VEC_INDEX(*temp, 0) * VEC_INDEX(*axis, 0);
    MAT_INDEX(*rotate, 1, 0) = 0 + VEC_INDEX(*temp, 0) * VEC_INDEX(*axis, 1) + s * VEC_INDEX(*axis, 2);
    MAT_INDEX(*rotate, 2, 0) = 0 + VEC_INDEX(*temp, 0) * VEC_INDEX(*axis, 2) - s * VEC_INDEX(*axis, 1);

    MAT_INDEX(*rotate, 0, 1) = 0 + VEC_INDEX(*temp, 1) * VEC_INDEX(*axis, 0) - s * VEC_INDEX(*axis, 2);
    MAT_INDEX(*rotate, 1, 1) = c + VEC_INDEX(*temp, 1) * VEC_INDEX(*axis, 1);
    MAT_INDEX(*rotate, 2, 1) = 0 + VEC_INDEX(*temp, 1) * VEC_INDEX(*axis, 2) + s * VEC_INDEX(*axis, 0);

    MAT_INDEX(*rotate, 0, 2) = 0 + VEC_INDEX(*temp, 2) * VEC_INDEX(*axis, 0) + s * VEC_INDEX(*axis, 1);
    MAT_INDEX(*rotate, 1, 2) = 0 + VEC_INDEX(*temp, 2) * VEC_INDEX(*axis, 1) - s * VEC_INDEX(*axis, 0);
    MAT_INDEX(*rotate, 2, 2) = c + VEC_INDEX(*temp, 2) * VEC_INDEX(*axis, 2);

    Matrix* result = alloc_matrix(0.0f, 4, 4, FALSE);
    Vector* mat0 = get_col(mat, 0);
    Vector* mat1 = get_col(mat, 1);
    Vector* mat2 = get_col(mat, 2);
    Vector* copy_vec;

    // result[0] = mat0 * MAT_INDEX(rotate, 0, 0) + mat1 * MAT_INDEX(rotate, 1, 0) + mat2 * MAT_INDEX(rotate, 2, 0);
    scalar_product_vector(*mat0, MAT_INDEX(*rotate, 0, 0));
    scalar_product_vector(*mat1, MAT_INDEX(*rotate, 1, 0));
    scalar_product_vector(*mat2, MAT_INDEX(*rotate, 2, 0));
    copy_vec = sum_vecs(3, *mat0, *mat1, *mat2);
    copy_vector_to_matrix_col(copy_vec, result, 0);
    deallocate_matrices(4, copy_vec, mat0, mat1, mat2);
    
    // Get again the cols
    mat0 = get_col(mat, 0);
    mat1 = get_col(mat, 1);
    mat2 = get_col(mat, 2);

    // result[1] = mat0 * MAT_INDEX(*rotate, 0, 1) + mat1 * MAT_INDEX(*rotate, 1, 1) + mat2 * MAT_INDEX(*rotate, 2, 1);
    scalar_product_vector(*mat0, MAT_INDEX(*rotate, 0, 1));
    scalar_product_vector(*mat1, MAT_INDEX(*rotate, 1, 1));
    scalar_product_vector(*mat2, MAT_INDEX(*rotate, 2, 1));
    copy_vec = sum_vecs(3, *mat0, *mat1, *mat2);
    copy_vector_to_matrix_col(copy_vec, result, 1);
    deallocate_matrices(4, copy_vec, mat0, mat1, mat2);

    // Get again the cols
    mat0 = get_col(mat, 0);
    mat1 = get_col(mat, 1);
    mat2 = get_col(mat, 2);

    // result[2] = mat0 * MAT_INDEX(*rotate, 0, 2) + mat1 * MAT_INDEX(*rotate, 1, 2) + mat2 * MAT_INDEX(*rotate, 2, 2);
    scalar_product_vector(*mat0, MAT_INDEX(*rotate, 0, 2));
    scalar_product_vector(*mat1, MAT_INDEX(*rotate, 1, 2));
    scalar_product_vector(*mat2, MAT_INDEX(*rotate, 2, 2));
    copy_vec = sum_vecs(3, *mat0, *mat1, *mat2);
    copy_vector_to_matrix_col(copy_vec, result, 2);
    deallocate_matrices(4, copy_vec, mat0, mat1, mat2);

    // result[3] = get_col(mat, 3);
    copy_vec = get_col(mat, 3);
    copy_vector_to_matrix_col(copy_vec, result, 3);
    
    // Deallocate all the unused resources
    deallocate_matrices(4, copy_vec, axis, temp, rotate);

    return result;
}

Matrix* translate_mat(Matrix* mat, Vector* vec) {
    Matrix* result = duplicate_matrix(mat);
    Vector* mat0 = get_col(mat, 0);
    Vector* mat1 = get_col(mat, 1);
    Vector* mat2 = get_col(mat, 2);
    Vector* mat3 = get_col(mat, 3);
    
    // result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
    scalar_product_vector(*mat0, VEC_INDEX(*vec, 0));
    scalar_product_vector(*mat1, VEC_INDEX(*vec, 1));
    scalar_product_vector(*mat2, VEC_INDEX(*vec, 2));
    Vector* copy_vec = sum_vecs(4, *mat0, *mat1, *mat2, *mat3);
    copy_vector_to_matrix_col(copy_vec, result, 3);

    // Deallocate all the unused resources
    deallocate_matrices(5, copy_vec, mat0, mat1, mat2, mat3);

    return result;
}

#endif //_TRANSFORMATION_H_