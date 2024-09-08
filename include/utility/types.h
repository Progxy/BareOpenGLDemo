#ifndef _TYPES_H_
#define _TYPES_H_

#define _USE_IMAGE_LIBRARY_
#include "../../libs/image_io.h"

typedef unsigned char bool;

typedef struct Array {
    void** data;
    unsigned int count;
} Array;

typedef struct ImageFile {
    char* file_path;
    Image image;
} ImageFile;

typedef struct TextureParams {
    unsigned short int mag_filter;
    unsigned short int min_filter;
    unsigned short int wrap_s;
    unsigned short int wrap_t;
} TextureParams;

typedef struct Matrix {
    unsigned int rows;
    unsigned int cols;
    float* data;
} Matrix;

typedef Matrix Vector;
typedef Matrix Quaternion;

typedef struct Camera {
    Vector camera_pos;
    Vector camera_front;
    Vector camera_up;
    float camera_speed;
} Camera;

#endif //_TYPES_H_
