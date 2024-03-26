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
#include "../../libs/gltf_header.h"

typedef struct Vertex {
    Vector position;
    Vector normal;
    Vector tex_coords;
} Vertex;

typedef struct ModelTexture {
    unsigned int id;
    const char* type;
} ModelTexture;  

typedef struct Shader {
    unsigned int vertex_shader;
    unsigned int fragment_shader;
} Shader;

typedef struct ModelMesh {
    unsigned int* VAO;
    unsigned int* VBO;
    unsigned int* EBO;
    Array vertices;
    Array textures;
    Array indices;
} ModelMesh;

typedef struct Model {
    Array meshes;
    const char* directory;
} Model;    

void setup_mesh(ModelMesh mesh) {
    glGenVertexArrays(1, mesh.VAO);
    glGenBuffers(1, mesh.VBO);
    glGenBuffers(1, mesh.EBO);

    glBindVertexArray(*(mesh.VAO));
    glBindBuffer(GL_ARRAY_BUFFER, *(mesh.VBO));

    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.count * sizeof(Vertex), *(mesh.vertices.data), GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(mesh.EBO));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.count * sizeof(unsigned int), *(mesh.indices.data), GL_STATIC_DRAW);

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

ModelMesh create_mesh(Array vertices, Array indices, Array textures) {
    ModelMesh mesh = {0};
    
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

void deallocate_mesh(ModelMesh mesh) {
    DEBUG_INFO("deallocating mesh...");
    deallocate_arr(mesh.vertices);
    deallocate_arr(mesh.textures);
    deallocate_arr(mesh.indices);
    free(mesh.VAO);
    free(mesh.VBO);
    free(mesh.EBO);
    return;
}

void draw_mesh(Shader shader, ModelMesh mesh) {
    unsigned int diffuse_nr = 1;
    unsigned int specular_nr = 1;

    for(unsigned int i = 0; i < mesh.textures.count; i++) {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

        // retrieve texture number (the N in diffuse_textureN)
        char* number;
        char* name = (char*) (GET_ELEMENT(ModelTexture*, mesh.textures, i) -> type);
        if (!strcmp(name, "texture_diffuse")) {
            number = num_to_str(diffuse_nr);
            diffuse_nr++;
        } else if (!strcmp(name, "texture_specular")) {
            number = num_to_str(specular_nr);
            specular_nr++;
        }

        char* material_id = (char*) calloc(1, sizeof(char));
        concat(3, &material_id, "sss", "material.", name, number);
        set_int(shader.vertex_shader, material_id, i, glUniform1i);
        glBindTexture(GL_TEXTURE_2D, GET_ELEMENT(ModelTexture*, mesh.textures, i) -> id);
        free(material_id);
        free(number);
    }

    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(*(mesh.VAO));
    glDrawElements(GL_TRIANGLES, mesh.indices.count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return;
}

void draw_model(Shader shader, Model model) {
    for (unsigned int i = 0; i < model.meshes.count; i++) {
        draw_mesh(shader, *GET_ELEMENT(ModelMesh*, model.meshes, i));
    }
    return;
}

static float* get_element_as_float(ArrayExtended arr_ext, unsigned int index) {
    if (index >= arr_ext.arr.count) return NULL;

    unsigned char size = elements_count[arr_ext.data_type];
    void* data = GET_ELEMENT(void*, arr_ext.arr, index);
    
    if ((arr_ext.component_type == BYTE) || (arr_ext.component_type == UNSIGNED_BYTE) || (arr_ext.component_type == SHORT) || (arr_ext.component_type == UNSIGNED_SHORT) || (arr_ext.component_type == UNSIGNED_INT)) {
        float* element = (float*) calloc(size, sizeof(float));
        for (unsigned int i = 0; i < size; ++i) {
            if (arr_ext.component_type == BYTE) element[i] = ((char*) data)[i];
            else if (arr_ext.component_type == UNSIGNED_BYTE) element[i] = ((unsigned char*) data)[i];
            else if (arr_ext.component_type == SHORT) element[i] = ((short int*) data)[i];
            else if (arr_ext.component_type == UNSIGNED_SHORT) element[i] = ((unsigned short int*) data)[i];
            else if (arr_ext.component_type == UNSIGNED_INT) element[i] = ((unsigned int*) data)[i];
        }
        return element;
    }
    
    return ((float*) data);
}

ModelMesh process_mesh(Mesh mesh) {
    ModelMesh model_mesh = {0};
    model_mesh.vertices = init_arr();
    model_mesh.textures = init_arr();
    model_mesh.indices = init_arr();

    for (unsigned int i = 0; i < mesh.vertices.arr.count; ++i) {
        Vertex vertex = {0};
        
        vertex.position = vec(3, 0.0f, 0.0f, 0.0f);
        float* position = get_element_as_float(mesh.vertices, i);
        for (unsigned int j = 0; j < elements_count[mesh.vertices.data_type]; ++j) {
            vertex.position.data[j] = position[j];
        }

        vertex.normal = vec(3, 0.0f, 0.0f, 0.0f);
        float* normal = get_element_as_float(mesh.normals, i);
        for (unsigned int j = 0; j < elements_count[mesh.normals.data_type]; ++j) {
            vertex.normal.data[j] = normal[j];
        }
        
        vertex.tex_coords = vec(2, 0.0f, 0.0f);   
        float* tex_coords = get_element_as_float(mesh.texture_coords, i);
        for (unsigned int j = 0; j < elements_count[mesh.texture_coords.data_type] && (tex_coords != NULL); ++j) {
            vertex.tex_coords.data[j] = tex_coords[j];
        }

        append_element(&(model_mesh.vertices), &vertex);
    }

    for (unsigned int i = 0; i < mesh.faces_count; ++i) {
        Face face = mesh.faces[i];
        for (unsigned int j = 0; j < topology_size[face.topology]; ++j) {
            append_element(&(model_mesh.indices), face.indices + j);
        }
    }

    return model_mesh;
}

void process_node(Array* meshes, Scene scene, Node node) {
    for (unsigned int i = 0; i < node.meshes_indices.count; ++i) {
        Mesh mesh = scene.meshes[*GET_ELEMENT(unsigned int*, node.meshes_indices, i)];
        ModelMesh model_mesh = process_mesh(mesh);
        append_element(meshes, &model_mesh);
    }

    for (unsigned int i = 0; i < node.children_count; ++i) {
        process_node(meshes, scene, node.childrens[i]);
    }

    return;
}

void load_model(char* path) {
    Scene scene = decode_gltf(path);
    
    if (scene.meshes == NULL) {
        printf("ERROR: error while decoding the model.\n");
        return;
    } 

    Model model = {0};
    model.directory = path; // remove the last part of the path 
    model.meshes = init_arr();

    process_node(&(model.meshes), scene, scene.root_node);

    return;
}

#endif //_MODEL_H_