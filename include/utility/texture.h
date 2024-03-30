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
#include "./types.h"
#include "./utils.h"

const unsigned short int values_filter[] = { GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR };
const unsigned short int values_wrap[] = { GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, GL_REPEAT };

void load_texture(const char* file_path, unsigned int* texture_id, TextureParams texture_params) {
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

    // Pass from Texture struct
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, values_wrap[texture_params.wrap_s]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, values_wrap[texture_params.wrap_t]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, values_filter[texture_params.min_filter]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, values_filter[texture_params.mag_filter]);

    deallocate_image(image);
    
    debug_info("texture successfully loaded\n");

    return;
}

#endif // _TEXTURE_H_