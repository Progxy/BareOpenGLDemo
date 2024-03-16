#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdio.h>
#include <stdlib.h>
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

#endif //_MODEL_H_