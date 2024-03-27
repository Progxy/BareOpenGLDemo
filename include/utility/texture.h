#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"

#endif //_STDLIB_DEF_

#include <dirent.h>
#define _USE_IMAGE_LIBRARY_
#include "../../libs/image_io.h"
#include "./types.h"
#include "./utils.h"

char** get_texture_images_path(char* path, unsigned int* file_count) {
    DIR* directory;
    
    if ((directory = opendir(path)) == NULL) {
        *file_count = 0;
        return NULL;
    }

    char** images_paths = (char**) calloc(1, sizeof(char*));
    struct dirent* file;
    *file_count = 0;

    while ((file = readdir(directory)) != NULL) {
        if ((!str_contains(file -> d_name, ".jpeg")) && (!str_contains(file -> d_name, ".jpg")) && (!str_contains(file -> d_name, ".png"))) {
            continue;
        }
        images_paths = (char**) realloc(images_paths, sizeof(char*) * (*file_count + 1));
        char* file_path = (char*) calloc(750, sizeof(char));
        int len = snprintf(file_path, 750, "%s%s", path, file -> d_name);
        file_path = (char*) realloc(file_path, sizeof(char) * len);
        images_paths[*file_count] = file_path;
        (*file_count)++;
    }

    closedir(directory);
    return images_paths;
}

void load_images(char* file_path, Array* image_arr) {
    unsigned int images_count = 0;
    char** images_paths = get_texture_images_path(file_path, &images_count);
    if (images_paths == NULL) {
        return;
    }
    
    for (unsigned int i = 0; i < images_count; ++i) {
        ImageFile* image_file = (ImageFile*) calloc(1, sizeof(ImageFile));
        image_file -> file_path = images_paths[i];
        debug_info("decoding image: '%s'...\n", image_file -> file_path);
        image_file -> image = decode_image(image_file -> file_path);
        append_element(image_arr, image_file);
    }
    
    return;
}

void deallocate_images(Array images_arr) {
    for (unsigned int i = 0; i < images_arr.count; ++i) {
        ImageFile* image_file = GET_ELEMENT(ImageFile*, images_arr, i);
        deallocate_image(image_file -> image);
        free(image_file -> file_path);
        free(image_file);
    }
    deallocate_arr(images_arr);
    return;
}

void load_texture(const char* file_path, unsigned int* texture_id) {
    glGenTextures(1, texture_id);

    debug_info("decoding image: '%s' ...\n", file_path);
    Image image = decode_image(file_path);

    if (image.error) {
        error_info("Texture failed to load at path: %s, with error: %s\n", file_path, err_codes[image.error]);
        deallocate_image(image);
        *texture_id = -1;
        return;
    }

    GLenum format;
    if (image.components == 1) format = GL_RED;
    else if (image.components == 3) format = GL_RGB;
    else if (image.components == 4) format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, *texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.decoded_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    deallocate_image(image);
    
    debug_info("texture successfully loaded\n");

    return;
}

bool load_textures(unsigned int vertex_shader, unsigned int* diffuse_map, unsigned int* specular_map) {
    // Load Texture
    load_texture("./assets/container2.png", diffuse_map);
    load_texture("./assets/container2_specular.png", specular_map);

    if (((int) *diffuse_map) == -1 || ((int) *specular_map) == -1) {
        return TRUE;
    }

    glUseProgram(vertex_shader);
    set_int(vertex_shader, "material.diffuse", 0, glUniform1i);
    set_int(vertex_shader, "material.specular", 1, glUniform1i);
    return FALSE;
}

#endif // _TEXTURE_H_