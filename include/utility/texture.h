#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#ifndef _STDLIB_DEF_
#define _STDLIB_DEF_

#include <stdio.h>
#include <stdlib.h>
#include "./GLFW/glfw3.h"

#endif //_STDLIB_DEF_

#define _USE_IMAGE_LIBRARY_
#include "../../libs/image_io.h"
#include "./utils.h"

void load_texture(const char* file_path, unsigned int* texture_id) {
    glGenTextures(1, texture_id);

    DEBUG_INFO("decoding the image...");
    Image image = decode_image(file_path);

    if (image.error) {
        printf("ERROR: Texture failed to load at path: %s, with error: %s\n", file_path, err_codes[image.error]);
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

    DEBUG_INFO("image successfully decoded");

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