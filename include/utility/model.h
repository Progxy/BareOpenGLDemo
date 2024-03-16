#ifndef _MODEL_H_
#define _MODEL_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"

#endif //_STDLIB_DEF_

#include <string.h>
#include "./utils.h"
#include "./matrix.h"

typedef struct Vertex {
    Vector position;
    Vector normal;
    Vector tex_coords;
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

#define GET_ELEMENT(type, arr, index) (*((type*) arr.struct_data[index]))
#define TO_STRING(num) (char) num + 48

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
    unsigned int* VAO;
    unsigned int* VBO;
    unsigned int* EBO;
    Array vertices;
    Array textures;
    Array indices;
} Mesh;

void setup_mesh(Mesh mesh) {
    glGenVertexArrays(1, mesh.VAO);
    glGenBuffers(1, mesh.VBO);
    glGenBuffers(1, mesh.EBO);

    glBindVertexArray(*(mesh.VAO));
    glBindBuffer(GL_ARRAY_BUFFER, *(mesh.VBO));

    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size * sizeof(Vertex), *(mesh.vertices.struct_data), GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(mesh.EBO));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size * sizeof(unsigned int), *(mesh.indices.struct_data), GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, tex_coords));

    glBindVertexArray(0);

    return;
}

Mesh create_mesh(Array vertices, Array indices, Array textures) {
    Mesh mesh = {0};
    
    mesh.vertices = vertices;
    mesh.indices = indices;
    mesh.textures = textures;
    
    mesh.VAO = (unsigned int*) calloc(1, sizeof(unsigned int));
    mesh.VBO = (unsigned int*) calloc(1, sizeof(unsigned int));
    mesh.EBO = (unsigned int*) calloc(1, sizeof(unsigned int));

    DEBUG_INFO("creating mesh...");

    setup_mesh(mesh);

    return mesh;
}

void deallocate_mesh(Mesh mesh) {
    DEBUG_INFO("deallocating mesh...");
    deallocate_arr(mesh.vertices);
    deallocate_arr(mesh.textures);
    deallocate_arr(mesh.indices);
    free(mesh.VAO);
    free(mesh.VBO);
    free(mesh.EBO);
    return;
}

void draw(Shader shader, Mesh mesh) {
    unsigned int diffuse_nr = 1;
    unsigned int specular_nr = 1;
    
    for(unsigned int i = 0; i < mesh.textures.size; i++) {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

        // retrieve texture number (the N in diffuse_textureN)
        char* number;
        char* name = GET_ELEMENT(Texture, mesh.textures, i).type;
        if (!strcmp(name, "texture_diffuse")) {
            number = TO_STRING(diffuse_nr);
            diffuse_nr++;
        } else if (!strcmp(name, "texture_specular")) {
            number = TO_STRING(specular_nr);
            specular_nr++;
        }

        char* material_id = (char*) calloc(1, sizeof(char));
        concat(3, *material_id, "sss", "material.", name, number);
        set_int(shader.vertex_shader, material_id, i, glUniform1i);
        glBindTexture(GL_TEXTURE_2D, GET_ELEMENT(Texture, mesh.textures, i).id);
        free(material_id);
    }

    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(*(mesh.VAO));
    glDrawElements(GL_TRIANGLES, mesh.indices.size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return;
}

#endif //_MODEL_H_