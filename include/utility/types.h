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

#endif //_TYPES_H_