#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>

#define TRUE 1
#define FALSE 0
#define MAT_INDEX(mat, row, col) (((mat).data)[((mat).cols * (row)) + (col)])
#define VEC_INDEX(vec, index) (((vec).data)[index])
#define alloc_vector(init_val, size) (alloc_matrix(init_val, size, 1, TRUE))

typedef struct Matrix {
    int rows;
    int cols;
    int isVec;
    float* data;
} Matrix;

typedef Matrix Vector;

void print_matrix(Matrix mat, const char* mat_name) {
    printf("-------------------------------------\n");
    printf("%s '%s': \n\n", mat.isVec ? "Vector" : "Matrix", mat_name);

    for (int row = 0; row < mat.rows; ++row) {
        for (int col = 0; col < mat.cols; ++col) {
            printf(" %f ", MAT_INDEX(mat, row, col));
        }
        printf("\n");
    }

    printf("-------------------------------------\n");

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

float get_vector_length(Vector vec) {
    // Check that vec is a vector and that has more than 3 rows
    assert(vec.rows >= 3 && vec.isVec);
    return sqrtf(powf(VEC_INDEX(vec, 0), 2.0f) + powf(VEC_INDEX(vec, 1), 2.0f) + powf(VEC_INDEX(vec, 2), 2.0f));
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

Matrix* sum_matrix(Matrix a, Matrix b, char subtract) {
    // Check that the number of rows and the number of columns of the matrices matches
    assert(a.rows == b.rows && a.cols == b.cols);

    // Create the result matrix
    Matrix* mat = alloc_matrix(0.0f, a.rows, a.cols, FALSE);

    // Sum the matrices and store the result inside the result matrix  
    for (int row = 0; row < a.rows; ++row) {
        for (int col = 0; col < a.cols; ++col) {
            MAT_INDEX(*mat, row, col) = MAT_INDEX(a, row, col) + (subtract * -1) * MAT_INDEX(b, row, col);
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

void scalar_product_matrix(Matrix mat, float scalar) {
    // Multiply each element with the scalar
    for (int row = 0; row < mat.rows; ++row) {
        for (int col = 0; col < mat.cols; ++col) {
            MAT_INDEX(mat, row, col) *= scalar;
        }
    }

    return;
}

void scalar_product_vector(Vector vec, float scalar) {
    // Multiply each element with the scalar
    for (int row = 0; row < vec.rows; ++row) {
        VEC_INDEX(vec, row) *= scalar;
    }

    return;
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

Matrix* duplicate_matrix(Matrix* mat) {
    Matrix* duplicate_mat = alloc_matrix(0.0f, mat -> rows, mat -> cols, mat -> isVec);
    
    // Copy the content of one matrix to the other
    for (int row = 0; row < mat -> rows; ++row) {
        for (int col = 0; col < mat -> cols; ++col) {
            MAT_INDEX(*duplicate_mat, row, col) = MAT_INDEX(*mat, row, col); 
        }
    }

    return duplicate_mat;
}

Vector* duplicate_vector(Vector* vec) {
    Vector* duplicate_vec = alloc_vector(0.0f, vec -> rows);
    
    // Copy the content of one matrix to the other
    for (int row = 0; row < vec -> rows; ++row) {
        VEC_INDEX(*duplicate_vec, row) = VEC_INDEX(*vec, row); 
    }

    return duplicate_vec;
}

Vector* get_col(Matrix* mat, int col) {
    Vector* row_vec = alloc_vector(0.0f, mat -> rows);

    for (int row = 0; row < mat -> rows; ++row) {
        VEC_INDEX(*row_vec, row) = MAT_INDEX(*mat, row, col);
    }

    return row_vec;
}

void copy_matrix(Matrix* src, Matrix* dest) {
    assert((src -> rows == dest -> rows) && (src -> cols == dest -> cols));

    for (int row = 0; row < src -> rows; ++row) {
        for (int col = 0; col < src -> cols; ++col) {
            MAT_INDEX(*dest, row, col) = MAT_INDEX(*src, row, col);
        }
    }

    return;
}

void copy_vector(Vector* src, Vector* dest) {
    assert(src -> rows == dest -> rows);

    for (int row = 0; row < src -> rows; ++row) {
        VEC_INDEX(*dest, row) = VEC_INDEX(*src, row);
    }

    return;
}

void copy_vector_to_matrix_col(Vector* src, Matrix* dest, int col) {
    assert(src -> rows == dest -> rows);

    for (int row = 0; row < src -> rows; ++row) {
        MAT_INDEX(*dest, row, col) = VEC_INDEX(*src, row);
    }

    return;
}

void deallocate_matrices(int len, ...) {
    va_list args;
    va_start(args, len);

    for (int i = 0; i < len; ++i) {
        Matrix* mat = va_arg(args, Matrix*);
        deallocate_matrix(mat);
    }

    va_end(args);

    return;
}

Vector* sum_vecs(int len, ...) {
    va_list args;

    va_start(args, len);

    Vector a = va_arg(args, Vector);

    // Create the result vector
    Vector* res = duplicate_vector(&a);

    // Sum each row and store the result inside the result vector
    for (int i = 0; i < len - 1; ++i) {   
        Vector vec = va_arg(args, Vector);

        // Assert that the vectors have the same size
        assert(vec.rows == res -> rows);

        for (int row = 0; row < res -> rows; ++row) {
            VEC_INDEX(*res, row) += VEC_INDEX(vec, row); 
        }
    }
    
    va_end(args);

    return res;
}

#endif // _MATRIX_H_