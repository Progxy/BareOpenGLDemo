#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define MAT_INDEX(mat, row, col) (((mat).data)[((mat).cols * (row)) + (col)])
#define VEC_INDEX(vec, index) (((vec).data)[index])
#define alloc_vector(init_val, size) (alloc_matrix(init_val, size, 1, 1))
#define MAP_MAT_INDEX(mat, index) (index % ((mat).rows * (mat).cols))
#define MAP_VEC_INDEX(vec, index) (index % (vec).rows)

typedef struct Matrix {
    int rows;
    int cols;
    int isVec;
    float* data;
} Matrix;

typedef Matrix Vector;

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
        res += VEC_INDEX(a, 1) + VEC_INDEX(b, 1);
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

#endif // _MATRIX_H_