#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdio.h>
#include <stdlib.h>
#include "./utils.h"
#include "./matrix.h"

typedef struct Vertex {
    Vector Position;
    Vector Normal;
    Vector TexCoords;
} Vertex;

typedef struct Texture {
    unsigned int id;
    const char* type;
} Texture;  

typedef struct Shader {
    unsigned int vertex_shader;
    unsigned int fragment_shader;
} Shader;

typedef struct Array {
    void** struct_data;
    unsigned int size;
} Array;

#define GET_ELEMENT(type, arr, index) *((type*) arr.struct_data[index])

void append_element(Array* arr, void* data) {
    arr -> struct_data = (void**) realloc(arr -> struct_data, sizeof(void*) * (arr -> size + 1));
    (arr -> struct_data)[arr -> size] = data;
    (arr -> size)++;
    return;
}

Array allocate_arr() {
    Array arr = { .size = 0 };
    arr.struct_data = (void**) calloc(arr.size, sizeof(void*));
    return arr;
}

void deallocate_arr(Array arr) {
    DEBUG_INFO("deallocating array...");
    free(arr.struct_data);
    return;
}

typedef struct Mesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    Array vertices;
    Array textures;
    Array indices;
} Mesh;

void setupMesh(Mesh mesh);
Mesh create_mesh(Array vertices, Array indices, Array textures);
void draw(Shader* shader);

#endif //_MODEL_H_