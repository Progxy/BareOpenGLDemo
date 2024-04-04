#ifndef _MODEL_H_
#define _MODEL_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"

#endif //_STDLIB_DEF_

#include "./utils.h"
#include "./texture.h"
#include "./transformation.h"
#include "./camera.h"
#include "../../libs/gltf_header.h"

typedef struct Vertex {
    float position[3];
    float normal[3];
    float tex_coords[2];
    float tangent[4];
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
    Vertex* vertices;
    unsigned int vertices_count;
    Array textures;
    unsigned int* indices;
    unsigned int indices_count;
    Matrix transformation_matrix;
    Vector translation_mat;
    Quaternion rotation_mat;
    Vector scale_mat;
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
    glBufferData(GL_ARRAY_BUFFER, (mesh -> vertices_count) * sizeof(Vertex), (mesh -> vertices) -> position, GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *(mesh -> EBO));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (mesh -> indices_count) * sizeof(unsigned int), mesh -> indices, GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);

    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, tex_coords));

    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, tangent));

    glBindVertexArray(0); // Unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO

    return;
}

void deallocate_mesh(ModelMesh mesh) {
    free(mesh.vertices);
    deallocate_arr(mesh.textures);
    free(mesh.indices);
    DEALLOCATE_MATRICES(mesh.transformation_matrix, mesh.translation_mat, mesh.rotation_mat, mesh.scale_mat);
    glDeleteVertexArrays(1, mesh.VAO);
    glDeleteBuffers(1, mesh.VBO);
    glDeleteBuffers(1, mesh.EBO);
    return;
}

void deallocate_model(Model* model) {
    debug_info("deallocating model...\n");
    for (unsigned int i = 0; i < model -> meshes.count; ++i) {
        deallocate_mesh(*GET_ELEMENT(ModelMesh*, model -> meshes, i));
    }
    free(model -> directory);
    free(model);
    return;
}

void draw_mesh(unsigned int shader, ModelMesh* mesh, Camera* camera) {
    unsigned int base_color_nr = 1;
    unsigned int metallic_roughness_nr = 1;
    unsigned int normal_nr = 1;
    unsigned int occlusion_nr = 1;
    unsigned int emissive_nr = 1;

    for (unsigned int i = 0; i < (mesh -> textures).count; ++i) {
        unsigned int number = 0;
        char* name = (char*) (GET_ELEMENT(ModelTexture*, mesh -> textures, i) -> type);
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding

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
        glBindTexture(GL_TEXTURE_2D, GET_ELEMENT(ModelTexture*, mesh -> textures, i) -> id);
        free(material_id);
    }
    
    set_vec(shader, "cam_pos", camera -> camera_pos.data, glUniform3fv);

    // draw mesh
    glBindVertexArray(*(mesh -> VAO));
    glDrawElements(GL_TRIANGLES, mesh -> indices_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind VAO

    // Set back to default
    glActiveTexture(GL_TEXTURE0);

    return;
}

void draw_model(unsigned int shader, Model* model, Camera* camera) {
    for (unsigned int i = 0; i < model -> meshes.count; ++i) {
        ModelMesh* mesh = GET_ELEMENT(ModelMesh*, model -> meshes, i);
        draw_mesh(shader, mesh, camera);
        set_matrix(shader, "transform", mesh -> transformation_matrix.data, glUniformMatrix4fv);
        set_matrix(shader, "translation", mesh -> translation_mat.data, glUniformMatrix4fv);
        set_matrix(shader, "rotation", mesh -> rotation_mat.data, glUniformMatrix4fv);
        set_matrix(shader, "scale", mesh -> scale_mat.data, glUniformMatrix4fv);
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
    TextureParams texture_params = (TextureParams) { 
        .wrap_s = normalize_wrap_values[texture.wrap_s], 
        .wrap_t = normalize_wrap_values[texture.wrap_t],
        .min_filter = normalize_filter_values[texture.min_filter],
        .mag_filter = normalize_filter_values[texture.mag_filter]
    };
    load_texture(texture.texture_path, &(model_texture -> id), texture_params);
    model_texture -> type = type;
    model_texture -> path = texture.texture_path;
    append_element(loaded_textures_arr, model_texture);
    return model_texture;
}

ModelMesh* process_mesh(Mesh mesh, Scene scene, Array* loaded_textures_arr) {
    ModelMesh* model_mesh = (ModelMesh*) calloc(1, sizeof(ModelMesh));
    model_mesh -> vertices = (Vertex*) calloc(1, sizeof(Vertex));
    model_mesh -> vertices_count = 0;
    model_mesh -> textures = init_arr();
    model_mesh -> indices = (unsigned int*) calloc(1, sizeof(unsigned int));
    model_mesh -> indices_count = 0;

    for (unsigned int i = 0; i < mesh.vertices.arr.count; ++i, ++(model_mesh -> vertices_count)) {
        model_mesh -> vertices = (Vertex*) realloc(model_mesh -> vertices, sizeof(Vertex) * (model_mesh -> vertices_count + 1));
        
        float* position = get_element_as_float(mesh.vertices, i);
        for (unsigned int j = 0; j < elements_count[mesh.vertices.data_type]; ++j) {
            ((model_mesh -> vertices)[model_mesh -> vertices_count]).position[j] = position[j];
        }

        float* normal = get_element_as_float(mesh.normals, i);
        for (unsigned int j = 0; j < elements_count[mesh.normals.data_type]; ++j) {
            ((model_mesh -> vertices)[model_mesh -> vertices_count]).normal[j] = normal[j];
        }        
        
        float* tangent = get_element_as_float(mesh.tangents, i);
        for (unsigned int j = 0; j < elements_count[mesh.tangents.data_type]; ++j) {
            ((model_mesh -> vertices)[model_mesh -> vertices_count]).tangent[j] = tangent[j];
        }
        
        float* tex_coords = get_element_as_float(mesh.texture_coords, i);
        for (unsigned int j = 0; j < elements_count[mesh.texture_coords.data_type] && (tex_coords != NULL); ++j) {
            ((model_mesh -> vertices)[model_mesh -> vertices_count]).tex_coords[j] = tex_coords[j];
        }
    }

    for (unsigned int i = 0; i < mesh.faces_count; ++i) {
        Face face = mesh.faces[i];
        for (unsigned int j = 0; j < topology_size[face.topology]; ++j, ++(model_mesh -> indices_count)) {
            model_mesh -> indices = (unsigned int*) realloc(model_mesh -> indices, sizeof(unsigned int) * (model_mesh -> indices_count + 1));
            (model_mesh -> indices)[model_mesh -> indices_count] = face.indices[j];
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

void process_node(Array* meshes, Scene scene, Node node, Array* loaded_textures_arr, Matrix parent_matrix) {
    Matrix translation_mat = create_identity_matrix(4);
    Matrix scale_mat = create_identity_matrix(4);

    Vector translation_vec = cast_vec(node.translation_vec, 3, FALSE);
    Quaternion rotation_quat = cast_quat(node.rotation_quat);
    Vector scale_vec = cast_vec(node.scale_vec, 3, FALSE);
    Matrix transformation_mat = cast_mat(node.transformation_matrix, 4, 4, FALSE);

    translate_mat(translation_mat, translation_vec, &translation_mat);
    Matrix rotation_mat = quat_to_mat4(rotation_quat);
    scale_matrix(scale_mat, scale_vec, &scale_mat);

    Matrix node_mat = alloc_quad_mat(0.0f, 4);
    DOT_PRODUCT_MATRIX(&node_mat, parent_matrix, transformation_mat, translation_mat, rotation_mat, scale_mat);
    DEALLOCATE_MATRICES(translation_vec, scale_vec, transformation_mat, rotation_quat);
    
    for (unsigned int i = 0; i < node.meshes_indices.count; ++i) {
        unsigned int mesh_index = *GET_ELEMENT(unsigned int*, node.meshes_indices, i);
        Mesh mesh = scene.meshes[mesh_index];
        ModelMesh* model_mesh = process_mesh(mesh, scene, loaded_textures_arr);
        model_mesh -> transformation_matrix = node_mat;
        model_mesh -> translation_mat = translation_mat;
        model_mesh -> rotation_mat = rotation_mat;
        model_mesh -> scale_mat = scale_mat;
        append_element(meshes, model_mesh);
    }

    for (unsigned int i = 0; i < node.children_count; ++i) {
        process_node(meshes, scene, node.childrens[i], loaded_textures_arr, node_mat);
    }

    return;
}

Model* load_model(char* path) {
    Scene scene = decode_gltf(path);
    
    if (scene.meshes == NULL) {
        error_info("error while decoding the model.\n");
        return NULL;
    } 

    Model* model = (Model*) calloc(1, sizeof(Model));
    Array loaded_textures_arr = init_arr();

    model -> directory = get_directory(path);
    model -> meshes = init_arr();
    Matrix id_mat = create_identity_matrix(4);
    process_node(&(model -> meshes), scene, scene.root_node, &loaded_textures_arr, id_mat);
    DEALLOCATE_MATRICES(id_mat);

    debug_info("model successfully loaded\n");

    return model;
}

#endif //_MODEL_H_