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
#include "./texture.h"
#include "../../libs/gltf_header.h"

typedef struct Vertex {
    Vector position;
    Vector normal;
    Vector tex_coords;
} Vertex;

typedef struct ModelTexture {
    unsigned int id;
    char* type;
    char* path;
} ModelTexture;  

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
    char* directory;
} Model;    

void setup_mesh(ModelMesh* mesh) {
    glGenVertexArrays(1, mesh -> VAO);
    glGenBuffers(1, mesh -> VBO);
    glGenBuffers(1, mesh -> EBO);

    glBindVertexArray(*(mesh -> VAO));
    glBindBuffer(GL_ARRAY_BUFFER, *(mesh -> VBO));

    glBufferData(GL_ARRAY_BUFFER, (mesh -> vertices).count * sizeof(Vertex), *((mesh -> vertices).data), GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(mesh -> EBO));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (mesh -> indices).count * sizeof(unsigned int), *((mesh -> indices).data), GL_STATIC_DRAW);

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

void deallocate_mesh(ModelMesh mesh) {
    deallocate_arr(mesh.vertices);
    deallocate_arr(mesh.textures);
    deallocate_arr(mesh.indices);
    glDeleteVertexArrays(1, mesh.VAO);
    glDeleteBuffers(1, mesh.VBO);
    glDeleteBuffers(1, mesh.EBO);
    return;
}

void deallocate_model(Model model) {
    debug_info("deallocating model...\n");
    for (unsigned int i = 0; i < model.meshes.count; ++i) {
        deallocate_mesh(*GET_ELEMENT(ModelMesh*, model.meshes, i));
    }
    free(model.directory);
    return;
}

void draw_mesh(unsigned int shader, ModelMesh mesh) {
    unsigned int base_color_nr = 1;
    unsigned int metallic_roughness_nr = 1;
    unsigned int normal_nr = 1;
    unsigned int occlusion_nr = 1;
    unsigned int emissive_nr = 1;

    for(unsigned int i = 0; i < mesh.textures.count; ++i) {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

        // retrieve texture number (the N in diffuse_textureN)
        unsigned int number = 0;
        char* name = (char*) (GET_ELEMENT(ModelTexture*, mesh.textures, i) -> type);
        if (!strcmp(name, "base_color_texture")) {
            number = base_color_nr;
            base_color_nr++;
        } else if (!strcmp(name, "metallic_roughness_texture")) {
            number = metallic_roughness_nr;
            metallic_roughness_nr++;
        } else if (!strcmp(name, "normal_texture")) {
            number = normal_nr;
            normal_nr++;
        } else if (!strcmp(name, "occlusion_texture")) {
            number = occlusion_nr;
            occlusion_nr++;
        } else if (!strcmp(name, "emissive_texture")) {
            number = emissive_nr;
            emissive_nr++;
        }

        char* material_id = (char*) calloc(1, sizeof(char));
        concat(3, &material_id, "ssu", "material.", name, number);
        set_int(shader, material_id, i, glUniform1i);
        glBindTexture(GL_TEXTURE_2D, GET_ELEMENT(ModelTexture*, mesh.textures, i) -> id);
        free(material_id);
    }
    
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(*(mesh.VAO));
    glDrawElements(GL_TRIANGLES, mesh.indices.count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    return;
}

void draw_model(unsigned int shader, Model model) {
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

ModelTexture* process_texture(Texture texture, char* type, Array* loaded_textures_arr) {
    ModelTexture* model_texture = (ModelTexture*) calloc(1, sizeof(ModelTexture));
    for (unsigned int i = 0; i < loaded_textures_arr -> count; ++i) {
        ModelTexture loaded_texture = *GET_ELEMENT(ModelTexture*, *loaded_textures_arr, i);
        if (!strcmp(loaded_texture.path, texture.texture_path)) {
            *model_texture = loaded_texture;
            return model_texture;
        }
    }
    load_texture(texture.texture_path, &(model_texture -> id));
    model_texture -> type = type;
    model_texture -> path = texture.texture_path;
    append_element(loaded_textures_arr, model_texture);
    return model_texture;
}

ModelMesh* process_mesh(Mesh mesh, Scene scene, Array* loaded_textures_arr) {
    ModelMesh* model_mesh = (ModelMesh*) calloc(1, sizeof(ModelMesh));
    model_mesh -> vertices = init_arr();
    model_mesh -> textures = init_arr();
    model_mesh -> indices = init_arr();

    for (unsigned int i = 0; i < mesh.vertices.arr.count; ++i) {
        Vertex* vertex = (Vertex*) calloc(1, sizeof(Vertex));
        
        vertex -> position = vec(3, 0.0f, 0.0f, 0.0f);
        float* position = get_element_as_float(mesh.vertices, i);
        for (unsigned int j = 0; j < elements_count[mesh.vertices.data_type]; ++j) {
            (vertex -> position).data[j] = position[j];
        }

        vertex -> normal = vec(3, 0.0f, 0.0f, 0.0f);
        float* normal = get_element_as_float(mesh.normals, i);
        for (unsigned int j = 0; j < elements_count[mesh.normals.data_type]; ++j) {
            (vertex -> normal).data[j] = normal[j];
        }
        
        vertex -> tex_coords = vec(2, 0.0f, 0.0f);   
        float* tex_coords = get_element_as_float(mesh.texture_coords, i);
        for (unsigned int j = 0; j < elements_count[mesh.texture_coords.data_type] && (tex_coords != NULL); ++j) {
            (vertex -> tex_coords).data[j] = tex_coords[j];
        }

        append_element(&(model_mesh -> vertices), vertex);
    }

    for (unsigned int i = 0; i < mesh.faces_count; ++i) {
        Face face = mesh.faces[i];
        for (unsigned int j = 0; j < topology_size[face.topology]; ++j) {
            append_element(&(model_mesh -> indices), face.indices + j);
        }
    }

    Material material = scene.materials[mesh.material_index];
    if (material.pbr_metallic_roughness.base_color_texture.texture_path != NULL) append_element(&(model_mesh -> textures), process_texture(material.pbr_metallic_roughness.base_color_texture, "base_color_texture", loaded_textures_arr));
    if (material.pbr_metallic_roughness.metallic_roughness_texture.texture_path != NULL) append_element(&(model_mesh -> textures), process_texture(material.pbr_metallic_roughness.metallic_roughness_texture, "metallic_roughness_texture", loaded_textures_arr));    
    if (material.normal_texture.texture.texture_path != NULL) append_element(&(model_mesh -> textures), process_texture(material.normal_texture.texture, "normal_texture", loaded_textures_arr));    
    if (material.occlusion_texture.texture.texture_path != NULL) append_element(&(model_mesh -> textures), process_texture(material.occlusion_texture.texture, "occlusion_texture", loaded_textures_arr));    
    if (material.emissive_texture.texture_path != NULL) append_element(&(model_mesh -> textures), process_texture(material.emissive_texture, "emissive_texture", loaded_textures_arr));

    model_mesh -> VAO = (unsigned int*) calloc(1, sizeof(unsigned int));
    model_mesh -> VBO = (unsigned int*) calloc(1, sizeof(unsigned int));
    model_mesh -> EBO = (unsigned int*) calloc(1, sizeof(unsigned int));

    setup_mesh(model_mesh);

    return model_mesh;
}

void process_node(Array* meshes, Scene scene, Node node, Array* loaded_textures_arr) {
    for (unsigned int i = 0; i < node.meshes_indices.count; ++i) {
        unsigned int mesh_index = *GET_ELEMENT(unsigned int*, node.meshes_indices, i);
        Mesh mesh = scene.meshes[mesh_index];
        ModelMesh* model_mesh = process_mesh(mesh, scene, loaded_textures_arr);
        append_element(meshes, model_mesh);
    }

    for (unsigned int i = 0; i < node.children_count; ++i) {
        process_node(meshes, scene, (node.childrens)[i], loaded_textures_arr);
    }

    return;
}

Model load_model(char* path) {
    Model model = {0};
    Scene scene = decode_gltf(path);
    
    if (scene.meshes == NULL) {
        error_info("error while decoding the model.\n");
        return model;
    } 

    Array loaded_textures_arr = init_arr();

    model.directory = get_directory(path);
    model.meshes = init_arr();
    process_node(&(model.meshes), scene, scene.root_node, &loaded_textures_arr);

    debug_info("model successfully loaded\n");

    return model;
}

#endif //_MODEL_H_