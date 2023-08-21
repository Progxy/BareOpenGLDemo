#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define PI 3.14159265359
#define MAT_INDEX(mat, row, col) (((mat).data)[((mat).cols * (row)) + (col)])
#define VEC_INDEX(vec, index) (((vec).data)[index])
#define alloc_vector(init_val, size) (alloc_matrix(init_val, size, 1, TRUE))
#define MAP_MAT_INDEX(mat, index) (index % ((mat).rows * (mat).cols))
#define MAP_VEC_INDEX(vec, index) (index % (vec).rows)
#define scalar_negation(mat) (scalar_product((mat), -1))
#define RADIANS_TO_DEGREE(angle) ((angle) * (180 / PI))
#define DEGREE_TO_RADIANS(angle) ((angle) * (PI / 180))

typedef struct Matrix {
    int rows;
    int cols;
    int isVec;
    float* data;
} Matrix;

typedef Matrix Vector;

void print_matrix(Matrix mat, const char* mat_name) {
    printf("\n-------------------------------------\n");
    printf("%s '%s': \n\n", mat.isVec ? "Vector" : "Matrix", mat_name);

    for (int row = 0; row < mat.rows; ++row) {
        for (int col = 0; col < mat.cols; ++col) {
            printf(" %f ", MAT_INDEX(mat, row, col));
        }
        printf("\n");
    }

    printf("\n-------------------------------------\n");

    return;
}

Matrix* alloc_matrix(float init_val, int rows, int cols, int isVec) {
    // Check if the given size is valid
    assert(rows >= 1 && cols >= 1);

    // Allocate the memory for the matrix
    Matrix* mat = malloc(sizeof(Matrix));
    mat -> rows = rows;
    mat -> cols = cols;
    mat -> data = malloc(sizeof(float) * rows * cols);
    mat -> isVec = isVec;

    // Init matrix values
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            MAT_INDEX(*mat, row, col) = init_val;
        }
    }

    return mat;
}

Matrix* create_identity_matrix(int size) {
    // Create the identity matrix
    Matrix* id_mat = alloc_matrix(0.0f, size, size, FALSE);

    // Add ones on the diagonal
    for (int i = 0; i < size; ++i) {
        MAT_INDEX(*id_mat, i, i) = 1.0f;
    }

    return id_mat;
}

Vector* sum_vectors(Vector a, Vector b) {
    // Assert that the vectors have the same size
    assert((a.rows == b.rows) && a.isVec && b.isVec);

    // Create the result vector
    Vector* res = alloc_vector(0.0f, a.rows);

    // Sum each row and store the result inside the result vector
    for (int i = 0; i < a.rows; ++i) {
        VEC_INDEX(*res, i) = VEC_INDEX(a, i) + VEC_INDEX(b, i); 
    }

    return res;
}

float get_vector_length(Vector vec) {
    // Check that vec is a vector and that has more than 3 rows
    assert(vec.rows >= 3 && vec.isVec);
    return sqrtf(powf(VEC_INDEX(vec, 0), 2.0f) + powf(VEC_INDEX(vec, 1), 2.0f));
}

void normalize_vector(Vector* vec) {
    // Check if vec is a vector
    assert(vec -> isVec);

    float len = get_vector_length(*vec);

    for (int i = 0; i < vec -> rows; ++i) {
        VEC_INDEX(*vec, i) = VEC_INDEX(*vec, i) / len;
    }

    return;
}

float dot_product_vector(Vector a, Vector b) {
    // Assert that the vectors have the same size
    assert((a.rows == b.rows) && a.isVec && b.isVec);

    // Multiply each row and sum them
    float res = 0.0f;
    for (int i = 0; i < a.rows; ++i) {
        res += VEC_INDEX(a, i) * VEC_INDEX(b, i);
    }

    return res;
}

Vector* cross_product(Vector a, Vector b) {
    // Check if the given vectors have more than one component
    assert(a.isVec && b.isVec && (a.rows == b.rows) && (a.rows == 3));
    
    Vector* res = alloc_vector(0.0f, 3);
    VEC_INDEX(*res, 0) = VEC_INDEX(a, 1) * VEC_INDEX(b, 2) - VEC_INDEX(a, 2) * VEC_INDEX(b, 1);
    VEC_INDEX(*res, 1) = VEC_INDEX(a, 2) * VEC_INDEX(b, 0) - VEC_INDEX(a, 0) * VEC_INDEX(b, 2);
    VEC_INDEX(*res, 2) = VEC_INDEX(a, 0) * VEC_INDEX(b, 1) - VEC_INDEX(a, 1) * VEC_INDEX(b, 0);

    return res;
}

Matrix* dot_product_matrix(Matrix a, Matrix b) {
    // Check if the cols of a is equal to the rows of b
    if (a.cols != b.rows) {
        print_matrix(a, "a");
        print_matrix(b, "b");
    }
    assert(a.cols == b.rows);

    // Allocate the result matrix
    Matrix* mat = alloc_matrix(0.0f, a.rows, b.cols, FALSE);
    
    // Multiply the two matrices and store the result inside the result matrix
    for (int row = 0; row < a.rows; ++row) {
        for (int col = 0; col < b.cols; ++col) {
            for (int i = 0; i < a.cols; ++i) {
                MAT_INDEX(*mat, row, col) += MAT_INDEX(a, row, i) * MAT_INDEX(b, i, col);
            }
        }
    }
    
    return mat;
}

Matrix* sum_matrix(Matrix a, Matrix b) {
    // Check that the number of rows and the number of columns of the matrices matches
    assert(a.rows == b.rows && a.cols == b.cols);

    // Create the result matrix
    Matrix* mat = alloc_matrix(0.0f, a.rows, a.cols, FALSE);

    // Sum the matrices and store the result inside the result matrix  
    for (int row = 0; row < a.rows; ++row) {
        for (int col = 0; col < a.cols; ++col) {
            MAT_INDEX(*mat, row, col) = MAT_INDEX(a, row, col) + MAT_INDEX(b, row, col);
        }
    }

    return mat;
}

void deallocate_matrix(Matrix* mat) {
    free(mat -> data);
    free(mat);
    return;
}

void scalar_sum(Matrix mat, float scalar) {
    // Sum each element with the scalar
    for (int row = 0; row < mat.rows; ++row) {
        for (int col = 0; col < mat.cols; ++col) {
            MAT_INDEX(mat, row, col) += scalar;
        }
    }

    return;
}

void scalar_product(Matrix mat, float scalar) {
    // Multiply each element with the scalar
    for (int row = 0; row < mat.rows; ++row) {
        for (int col = 0; col < mat.cols; ++col) {
            MAT_INDEX(mat, row, col) *= scalar;
        }
    }

    return;
}

Matrix* scale_matrix(Vector scaling_vec) {
    Matrix* mat = create_identity_matrix(scaling_vec.rows);
    Matrix* scaling_matrix = dot_product_matrix(*mat, scaling_vec);
    deallocate_matrix(mat);

    return scaling_matrix;
}

Matrix* translate_matrix(Vector translation_vec) {
    Matrix* translation_matrix = create_identity_matrix(translation_vec.rows);
    
    for (int i = 0; i < translation_vec.rows; ++i) {
        MAT_INDEX(*translation_matrix, i, 3) = VEC_INDEX(translation_vec, i);
    }

    return translation_matrix;
}

Matrix* rotation_x_matrix(float angle, int size) {
    Matrix* rotation_x_mat = create_identity_matrix(size);
    MAT_INDEX(*rotation_x_mat, 1, 1) = RADIANS_TO_DEGREE(cosf(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_x_mat, 1, 2) = - RADIANS_TO_DEGREE(sinf(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_x_mat, 2, 1) = RADIANS_TO_DEGREE(sinf(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_x_mat, 2, 2) = RADIANS_TO_DEGREE(cosf(DEGREE_TO_RADIANS(angle)));

    return rotation_x_mat;
}

Matrix* rotation_y_matrix(float angle, int size) {
    Matrix* rotation_y_mat = create_identity_matrix(size);
    MAT_INDEX(*rotation_y_mat, 0, 0) = RADIANS_TO_DEGREE(cosf(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_y_mat, 0, 2) = RADIANS_TO_DEGREE(sinf(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_y_mat, 2, 0) = - RADIANS_TO_DEGREE(sinf(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_y_mat, 2, 2) = RADIANS_TO_DEGREE(cosf(DEGREE_TO_RADIANS(angle)));

    return rotation_y_mat;
} 

Matrix* rotation_z_matrix(float angle, int size) {
    Matrix* rotation_z_mat = create_identity_matrix(size);
    MAT_INDEX(*rotation_z_mat, 0, 0) = RADIANS_TO_DEGREE(cosf(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_z_mat, 0, 1) = - RADIANS_TO_DEGREE(sinf(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_z_mat, 1, 0) = RADIANS_TO_DEGREE(sinf(DEGREE_TO_RADIANS(angle)));
    MAT_INDEX(*rotation_z_mat, 1, 1) = RADIANS_TO_DEGREE(cosf(DEGREE_TO_RADIANS(angle)));

    return rotation_z_mat;
}

Vector* vec3(float a, float b, float c) {
    // Create the vector
    Vector* vec = alloc_vector(0.0f, 3);
    
    // Insert data
    VEC_INDEX(*vec, 0) = a;
    VEC_INDEX(*vec, 1) = b;
    VEC_INDEX(*vec, 2) = c;

    return vec;
}

Vector* vec4(float a, float b, float c, float d) {
    // Create the vector
    Vector* vec = alloc_vector(0.0f, 4);
    
    // Insert data
    VEC_INDEX(*vec, 0) = a;
    VEC_INDEX(*vec, 1) = b;
    VEC_INDEX(*vec, 2) = c;
    VEC_INDEX(*vec, 3) = d;

    return vec;
}

#endif // _MATRIX_H_