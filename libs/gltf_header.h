#ifndef _GLTF_HEADER_H
#define _GLTF_HEADER_H

#include "../include/utility/types.h"

typedef unsigned char bool;

typedef enum Filter { NEAREST = 9728, LINEAR, NEAREST_MIPMAP_NEAREST = 9984, LINEAR_MIPMAP_NEAREST, NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_LINEAR } Filter;
typedef enum Topology { POINTS, LINES, LINE_LOOP, LINE_STRIP, TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN } Topology;
typedef enum ComponentType { BYTE, UNSIGNED_BYTE, SHORT, UNSIGNED_SHORT, UNSIGNED_INT, FLOAT } ComponentType;
typedef enum Wrap { CLAMP_TO_EDGE = 33071, MIRRORED_REPEAT = 33648, REPEAT = 10497 } Wrap;
typedef enum DataType { SCALAR, VEC2, VEC3, VEC4, MAT2, MAT3, MAT4 } DataType;

const char* topology_type[] = { "POINTS", "LINES", "LINE_LOOP", "LINE_STRIP", "TRIANGLES", "TRIANGLE_STRIP", "TRIANGLE_FAN" };
const char* component_type_str[] = { "BYTE", "UNSIGNED_BYTE", "SHORT", "UNSIGNED_SHORT", "", "UNSIGNED_INT", "FLOAT" };
const char* data_type_str[] = { "SCALAR", "VEC2", "VEC3", "VEC4", "MAT2", "MAT3", "MAT4" };

const unsigned short int normalize_filter_values[] = { 9728, 9728, 9982, 9982, 9982, 9982 };
const unsigned short int normalize_wrap_values[] = { 33071, 33647, 10495 };

const unsigned char byte_lengths[] = { sizeof(char), sizeof(unsigned char), sizeof(short int), sizeof(unsigned short int), 0, sizeof(unsigned int), sizeof(float) };
const unsigned char elements_count[] = { 1, 2, 3, 4, 4, 9, 16 };
const unsigned char topology_size[] = { 1, 2, 2, 2, 3, 3, 3};

typedef struct ArrayExtended {
    Array arr;
    DataType data_type;
    ComponentType component_type;
} ArrayExtended;

typedef ArrayExtended Vertices;
typedef ArrayExtended Normals;
typedef ArrayExtended Tangents;
typedef ArrayExtended TextureCoords;

typedef struct Face {
    unsigned int* indices;
    Topology topology;
} Face;

typedef struct Mesh {
    Vertices vertices; // equivalent to the POSITION attribute of glTF meshes
    Normals normals;
    Tangents tangents;
    TextureCoords texture_coords;
    Face* faces;
    unsigned int faces_count;
    unsigned int material_index;
} Mesh;

typedef struct Node {
    Array meshes_indices;
    struct Node* childrens;
    unsigned int children_count;
    float transformation_matrix[16];
    float translation_vec[3];
    float rotation_quat[4];\
    float scale_vec[3];
} Node;

typedef struct Texture {
    char* texture_path;
    Filter mag_filter;
    Filter min_filter;
    Wrap wrap_s;
    Wrap wrap_t;
    unsigned int tex_coord;
} Texture;

typedef struct PbrMetallicRoughness {
    float* base_color_factor;
    Texture base_color_texture;
    Texture metallic_roughness_texture;
    float metallic_factor;
    float roughness_factor;
} PbrMetallicRoughness;

typedef struct NormalTextureInfo {
    Texture texture;
    unsigned int scale;
} NormalTextureInfo;

typedef struct OcclusionTextureInfo {
    Texture texture;
    unsigned int strength;
} OcclusionTextureInfo;

typedef struct Material {
    PbrMetallicRoughness pbr_metallic_roughness;
    NormalTextureInfo normal_texture;
    OcclusionTextureInfo occlusion_texture;
    Texture emissive_texture;
    float* emissive_factor;
    char* alpha_mode;
    float alpha_cutoff;
    bool double_sided;
} Material;

typedef struct Scene {
    Node root_node;
    Mesh* meshes;
    unsigned int meshes_count;
    Material* materials;
    unsigned int materials_count;
} Scene;

Scene decode_gltf(char* path);

#endif //_GLTF_HEADER_H
