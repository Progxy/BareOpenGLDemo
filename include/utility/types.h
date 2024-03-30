#ifndef _TYPES_H_
#define _TYPES_H_

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

#endif //_TYPES_H_