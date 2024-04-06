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
#define QUAT_INDEX(quat, index) (((quat).data)[index])
#define IS_VEC(mat) (mat.rows == 1 || mat.cols == 1)
#define IS_QUAT(mat) ((mat.rows == 1 && mat.cols == 4) || (mat.rows = 4 && mat.cols == 1))
#define VEC_SIZE(vec) ((vec.rows == 1) ? vec.cols : vec.rows)
#define alloc_vector(init_val, size) alloc_matrix(init_val, size, 1)
#define alloc_temp_vector(init_val, size) alloc_temp_matrix(init_val, size, 1)
#define cast_vec(vec_data, size, is_row_major) cast_mat(vec_data, is_row_major ? size : 1, !is_row_major ? size : 1, is_row_major)
#define cast_quat(quat_data) cast_mat(quat_data, 4, 1, TRUE)
#define alloc_quad_mat(init_val, size) alloc_matrix(init_val, size, size)
#define DEALLOCATE_MATRICES(...) deallocate_matrices(sizeof((Matrix[]){__VA_ARGS__}) / sizeof(Matrix), __VA_ARGS__)
#define VEC(...) vec(sizeof((float[]) {__VA_ARGS__}) / sizeof(float), __VA_ARGS__)
#define SUM_MATRICES(...) sum_matrices(sizeof((Matrix[]) {*__VA_ARGS__}) / sizeof(Matrix) - 1, __VA_ARGS__)
#define DOT_PRODUCT_MATRIX(...) dot_product_matrix(sizeof((Matrix[]) {*__VA_ARGS__}) / sizeof(Matrix) - 1, __VA_ARGS__)
#define PRINT_MAT(mat) print_matrix(mat, #mat)

typedef struct Matrix {
    unsigned int rows;
    unsigned int cols;
    float* data;
} Matrix;

typedef Matrix Vector;
typedef Matrix Quaternion;

/* DECLARATIONS */

void print_matrix(Matrix mat, const char* mat_name);
void reshape_matrix(unsigned int rows, unsigned int cols, Matrix* mat); 
Matrix alloc_matrix(float init_val, unsigned int rows, unsigned int cols); 
void gc_dispose();
Matrix alloc_temp_matrix(float init_val, unsigned int rows, unsigned int cols);
void deallocate_matrices(int len, ...);
Matrix create_identity_matrix(unsigned int size); 
Matrix get_col(Matrix mat, unsigned int col); 
void copy_vector_to_matrix_col(Vector src, Matrix dest, unsigned int col); 
float get_vector_length(Vector vec);
void copy_matrix(Matrix src, Matrix* dest);
void normalize_vector(Vector vec, Vector* normalized_vec);
Vector cross_product(Vector a, Vector b);
Vector vec(int len, ...); 
void scalar_sum_matrix(Matrix src, float scalar, Matrix* dest);
void scalar_product_matrix(Matrix src, float scalar, Matrix* dest);
Matrix negate(Matrix a);
void sum_matrices(int len, ...); 
void dot_product_matrix(int len, ...); 
void transpose_matrix(Matrix src, Matrix* dest); 
Matrix cast_mat(float* mat_data, unsigned int rows, unsigned int cols, bool is_row_major);
Matrix quat_to_mat4(Quaternion quat);

/* ----------------------------------------------- */

// NOTE: The matrices are row-major order, while the vectors are column-major order

void print_matrix(Matrix mat, const char* mat_name) {
    printf("-------------------------------------\n");
    printf("%s '%s': \n", IS_VEC(mat) ? IS_QUAT(mat) ? "Quaternion" : "Vector" : "Matrix", mat_name);

    for (unsigned int row = 0; row < mat.rows; ++row) {
        for (unsigned int col = 0; col < mat.cols; ++col) {
            printf(" %e ", MAT_INDEX(mat, row, col));
        }
        printf("\n");
    }

    printf("-------------------------------------\n");

    return;
}

void reshape_matrix(unsigned int rows, unsigned int cols, Matrix* mat) {
    mat -> rows = rows;
    mat -> cols = cols;
    mat -> data = (float*) realloc(mat -> data, cols * rows * sizeof(float));
    if (mat -> data == NULL) {
        printf("MATRIX::RESHAPE_MATRIX: Failed to reallocate the matrix data!\n");
        return;
    }
    return;
}

Matrix alloc_matrix(float init_val, unsigned int rows, unsigned int cols) {
    // Check if the given size is valid
    assert(rows >= 1 && cols >= 1);

    // Allocate the memory for the matrix
    Matrix mat = {};
    mat.rows = rows;
    mat.cols = cols;
    mat.data = calloc(rows * cols, sizeof(float));

    // Init matrix values
    for (unsigned int row = 0; row < rows; ++row) {
        for (unsigned int col = 0; col < cols; ++col) {
            MAT_INDEX(mat, row, col) = init_val;
        }
    }

    return mat;
}

void gc_dispose() {
    Matrix mat = alloc_temp_matrix(0.0f, 0, 0);
    (void) mat;
    return;
}

Matrix alloc_temp_matrix(float init_val, unsigned int rows, unsigned int cols) {
    static float** gc_pointer;
    static unsigned int count = 0; 

    if (rows == 0 && cols == 0) {
        free(gc_pointer);
        count = 0;
        return (Matrix) {};
    }

    if (count == 0) {
        gc_pointer = (float**) calloc(1, sizeof(float*));
        Matrix mat = alloc_matrix(init_val, rows, cols);
        gc_pointer[count] = mat.data;
        count++;
        return mat;
    }

    gc_pointer = (float**) realloc(gc_pointer, (count + 1) * sizeof(float*));
    Matrix mat = alloc_matrix(init_val, rows, cols);
    gc_pointer[count] = mat.data;
    count++;
    return mat;
}

void deallocate_matrices(int len, ...) {
    va_list args;
    va_start(args, len);

    for (int i = 0; i < len; ++i) {
        Matrix mat = va_arg(args, Matrix);
        free(mat.data);
    }

    va_end(args);

    return;
}

Matrix create_identity_matrix(unsigned int size) {
    // Reshape the id matrix
    Matrix id_mat = alloc_matrix(0.0f, size, size);

    // Add ones on the diagonal
    for (unsigned int i = 0; i < size; ++i) {
        MAT_INDEX(id_mat, i, i) = 1.0f;
    }

    return id_mat;
}

Matrix get_col(Matrix mat, unsigned int col) {
    assert(col < mat.cols);
    Vector row_vec = alloc_temp_vector(0.0f, mat.rows);

    for (unsigned int row = 0; row < mat.rows; ++row) {
        VEC_INDEX(row_vec, row) = MAT_INDEX(mat, row, col);
    }

    return row_vec;
}

void copy_vector_to_matrix_col(Vector src, Matrix dest, unsigned int col) {
    unsigned int vec_size = VEC_SIZE(src);
    assert(vec_size == dest.rows && col < dest.cols);

    for (unsigned int i = 0; i < vec_size; ++i) {
        MAT_INDEX(dest, i, col) = VEC_INDEX(src, i);
    }

    return;
}

// Calculate the euclidean norm
float get_vector_length(Vector vec) {
    // Check that vec is a vector
    assert(IS_VEC(vec));
    
    // Init the sum 
    float sum = 0.0f;

    unsigned int vec_size = VEC_SIZE(vec);

    for (unsigned int i = 0; i < vec_size; ++i) {
        sum += powf(VEC_INDEX(vec, i), 2.0f);
    }

    return sqrtf(sum);
}

void copy_matrix(Matrix src, Matrix* dest) {
    reshape_matrix(src.rows, src.cols, dest);

    for (unsigned int row = 0; row < src.rows; ++row) {
        for (unsigned int col = 0; col < src.cols; ++col) {
            MAT_INDEX(*dest, row, col) = MAT_INDEX(src, row, col);
        }
    }

    return;
}

void normalize_vector(Vector vec, Vector* normalized_vec) {
    // Check if vec is a vector
    assert(IS_VEC(vec));
    
    unsigned int vec_size = VEC_SIZE(vec);
    Vector temp = alloc_vector(0.0f, vec_size);

    float len = get_vector_length(vec);

    for (unsigned int i = 0; i < vec_size; ++i) {
        VEC_INDEX(temp, i) = VEC_INDEX(vec, i) / len;
    }

    // Copy the temp vector back to the normalized one
    copy_matrix(temp, normalized_vec);

    // Deallocate unused vector
    DEALLOCATE_MATRICES(temp);

    return;
}

Vector cross_product(Vector a, Vector b) {
    // Check if the given vectors have more than one component
    assert(IS_VEC(a) && IS_VEC(b) && (a.rows == b.rows) && (a.rows == 3));
    
    Matrix result = alloc_temp_vector(0.0f, a.rows);

    VEC_INDEX(result, 0) = VEC_INDEX(a, 1) * VEC_INDEX(b, 2) - VEC_INDEX(a, 2) * VEC_INDEX(b, 1);
    VEC_INDEX(result, 1) = VEC_INDEX(a, 2) * VEC_INDEX(b, 0) - VEC_INDEX(a, 0) * VEC_INDEX(b, 2);
    VEC_INDEX(result, 2) = VEC_INDEX(a, 0) * VEC_INDEX(b, 1) - VEC_INDEX(a, 1) * VEC_INDEX(b, 0);

    return result;
}

Vector vec(int len, ...) {
    va_list args;

    va_start(args, len);

    // Init vector
    Vector vec = alloc_vector(0.0f, len);

    // Set each row
    for (int i = 0; i < len; ++i) {   
        VEC_INDEX(vec, i) = (float) va_arg(args, double);
    }
    
    va_end(args);

    return vec;
}

void scalar_sum_matrix(Matrix src, float scalar, Matrix* dest) {
    Matrix temp = alloc_matrix(0.0f, 1, 1);
    copy_matrix(src, &temp);

    // Sum each element with the scalar
    for (unsigned int row = 0; row < src.rows; ++row) {
        for (unsigned int col = 0; col < src.cols; ++col) {
            MAT_INDEX(temp, row, col) += scalar;
        }
    }

    copy_matrix(temp, dest);
    DEALLOCATE_MATRICES(temp);

    return;
}

void scalar_product_matrix(Matrix src, float scalar, Matrix* dest) {
    Matrix temp = alloc_matrix(0.0f, 1, 1);
    copy_matrix(src, &temp);

    // Multiply each element with the scalar
    for (unsigned int row = 0; row < src.rows; ++row) {
        for (unsigned int col = 0; col < src.cols; ++col) {
            MAT_INDEX(temp, row, col) *= scalar;
        }
    }

    copy_matrix(temp, dest);
    DEALLOCATE_MATRICES(temp);

    return;
}

Matrix negate(Matrix a) {
    Matrix neg = alloc_temp_matrix(0.0f, 1, 1);
    scalar_product_matrix(a, -1.0f, &neg);
    return neg;
}

void sum_matrices(int len, ...) {
    va_list args;

    va_start(args, len);

    // Retrieve the result matrix
    Matrix* dest = va_arg(args, Matrix*);
    Matrix a = va_arg(args, Matrix);

    Matrix temp = alloc_matrix(0.0f, a.rows, a.cols);

    // Sum each matrix and store the result inside the destination matrix
    for (int i = 0; i < len; ++i) {   
        Matrix b = (i == 0) ? a : va_arg(args, Matrix);

        // Assert that the matrices have the same shape
        assert(b.rows == temp.rows && b.cols == temp.cols);

        // Sum the matrices and store the result inside the result matrix  
        for (unsigned int row = 0; row < a.rows; ++row) {
            for (unsigned int col = 0; col < a.cols; ++col) {
                MAT_INDEX(temp, row, col) += MAT_INDEX(b, row, col);
            }
        }
    }

    va_end(args);

    // Copy the temp matrix back to the destination matrix
    copy_matrix(temp, dest);

    // Deallocate the temp matrix
    DEALLOCATE_MATRICES(temp);

    return;
}

void dot_product_matrix(int len, ...) {
    va_list args;

    va_start(args, len);

    // Retrieve the result matrix
    Matrix* dest = va_arg(args, Matrix*);
    Matrix a = va_arg(args, Matrix);

    // Create the temp matrix that will hold the result
    float** temp = (float**) calloc(a.rows, sizeof(float*));
    unsigned int temp_rows = a.rows;
    unsigned int temp_cols = a.cols;

    // Copy the value of a into temp
    for (unsigned int row = 0; row < temp_rows; ++row) {
        temp[row] = (float*) calloc(temp_cols, sizeof(float));
        for (unsigned int col = 0; col < temp_cols; ++col) {
            temp[row][col] = MAT_INDEX(a, row, col);
        }
    }

    // Multiply each matrix and store the result inside the destination matrix
    for (int i = 0; i < len - 1; ++i) {   
        Matrix b = va_arg(args, Matrix);

        // Assert that the matrices can be multiplied (temp x b)
        assert(temp_cols == b.rows);
    
        // Multiply the two matrices and store the result inside the result matrix
        for (unsigned int row = 0; row < temp_rows; ++row) {
            // Init the row array that will hold the resulting row for each multiplication
            float* new_row = (float*) calloc(b.cols, sizeof(float));

            for (unsigned int col = 0; col < b.cols; ++col) {

                for (unsigned int i = 0; i < temp_cols; ++i) {
                    new_row[col] += temp[row][i] * MAT_INDEX(b, i, col);
                }
            }

            // Copy the new row into the old one
            temp[row] = new_row;
        }
        
        // Set the new number of temp cols
        temp_cols = b.cols;
    }
    
    va_end(args);

    // Copy the temp matrix into the destination matrix
    reshape_matrix(temp_rows, temp_cols, dest);
    for (unsigned int row = 0; row < temp_rows; ++row) {
        for (unsigned int col = 0; col < temp_cols; ++col) {
            MAT_INDEX(*dest, row, col) = temp[row][col];
        }
        // Deallocate copied row
        free(temp[row]);
    }

    free(temp);

    return;
}

void transpose_matrix(Matrix src, Matrix* dest) {
    reshape_matrix(src.cols, src.rows, dest);

    for (unsigned int row = 0; row < dest -> rows; ++row) {
        for (unsigned int col = 0; col < dest -> cols; ++col) {
            MAT_INDEX(*dest, row, col) = MAT_INDEX(src, col, row);
        }
    }

    return;
}

Matrix cast_mat(float* mat_data, unsigned int rows, unsigned int cols, bool is_row_major) {
    Matrix mat = alloc_matrix(0.0f, rows, cols);
    for (unsigned int r = 0; r < rows; ++r) {
        for (unsigned int c = 0; c < cols; ++c) {
            if (is_row_major) mat.data[r * cols + c] = mat_data[r * cols + c];
            else mat.data[r * cols + c] = mat_data[c * rows + r];
        }
    }
    return mat;
}

Matrix quat_to_mat4(Quaternion quat) {
    Matrix mat = create_identity_matrix(4);
    
    // First row
    MAT_INDEX(mat, 0, 0) = 2 * (powf(QUAT_INDEX(quat, 0), 2.0f) + powf(QUAT_INDEX(quat, 1), 2.0f)) - 1.0f;
    MAT_INDEX(mat, 0, 1) = 2 * (QUAT_INDEX(quat, 1) * QUAT_INDEX(quat, 2) - QUAT_INDEX(quat, 0) * QUAT_INDEX(quat, 3));
    MAT_INDEX(mat, 0, 2) = 2 * (QUAT_INDEX(quat, 1) * QUAT_INDEX(quat, 3) + QUAT_INDEX(quat, 0) * QUAT_INDEX(quat, 2));
    
    // Second row
    MAT_INDEX(mat, 1, 0) = 2 * (QUAT_INDEX(quat, 1) * QUAT_INDEX(quat, 2) + QUAT_INDEX(quat, 0) * QUAT_INDEX(quat, 3));
    MAT_INDEX(mat, 1, 1) = 2 * (powf(QUAT_INDEX(quat, 0), 2.0f) + powf(QUAT_INDEX(quat, 2), 2.0f)) - 1.0f;
    MAT_INDEX(mat, 1, 2) = 2 * (QUAT_INDEX(quat, 2) * QUAT_INDEX(quat, 3) - QUAT_INDEX(quat, 0) * QUAT_INDEX(quat, 1));

    // Third row
    MAT_INDEX(mat, 2, 0) = 2 * (QUAT_INDEX(quat, 1) * QUAT_INDEX(quat, 3) - QUAT_INDEX(quat, 0) * QUAT_INDEX(quat, 2));
    MAT_INDEX(mat, 2, 1) = 2 * (QUAT_INDEX(quat, 2) * QUAT_INDEX(quat, 3) + QUAT_INDEX(quat, 0) * QUAT_INDEX(quat, 1));
    MAT_INDEX(mat, 2, 2) = 2 * (powf(QUAT_INDEX(quat, 0), 2.0f) + powf(QUAT_INDEX(quat, 3), 2.0f)) - 1.0f;
    
    return mat;
}

#endif // _MATRIX_H_