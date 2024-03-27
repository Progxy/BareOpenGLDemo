#ifndef _UTILS_H_
#define _UTILS_H_

#define __USE_MISC 
#include <math.h>
#include <stdarg.h>
#include "./types.h"

#define TRUE 1
#define FALSE 0
#define WIDTH 800
#define HEIGHT 600
#define SENSITIVITY 0.1f
#define DEBUG_INFO(debug_str) printf("DEBUG_INFO: %s\n", debug_str)
#define CLIP(val, min, max) ((val > max) ? max : (val < min ? min : val))
#define GET_ELEMENT(type, arr, index) ((type) (((arr).data)[index]))

Array init_arr();
void append_element(Array* arr, void* element);
void deallocate_arr(Array arr);

char* get_directory(char* path) {
    char* directory = (char*) calloc(350, sizeof(char));
    int new_len = 0;
    for (new_len = strlen(path); new_len >= 0 && path[new_len] != '/'; --new_len) { }
    for (int i = 0; i < new_len; ++i) {
        directory[i] = path[i];
    }
    directory[new_len] = '\0';
    directory = (char*) realloc(directory, sizeof(char) * (new_len + 1));
    printf("DEBUG_INFO: old_path: '%s', new_path: '%s'\n", path, directory);
    return directory;
}

void set_int(unsigned int shader, const char* obj_name, int obj_data, void (*uniform_value)(GLint, GLint)) {
    unsigned int object = glGetUniformLocation(shader, obj_name);
    return (*uniform_value)(object, obj_data);
}

float deg_to_rad(float deg) {
    return deg * (M_PI / 180);
}

float rad_to_deg(float rad) {
    return rad * (180 / M_PI);
}

void print_float_bits(float* val) {
    unsigned char* val_uc = (unsigned char*) val;
    for (int j = 0; j < 4; ++j) {    
        for (int i = 0; i < 8; i++) {
            printf("%d", ((*(val_uc + j)) & (1 << i)) ? 1 : 0);
        }
        printf(" ");
    }
    printf("\n");
    return;
}

char* num_to_str(unsigned int val) {
    unsigned int digits = 0;
    long double copy_val = val;
    
    while (copy_val >= 1.0L) {
        copy_val /= 10.0L;
        digits++;
    }
    
    digits = (!digits) ? 1 : digits;
    char* str = (char*) calloc(digits, sizeof(char));
    if (!digits) {
        str[0] = '0';
    }
    
    for (int i = digits - 1, ind = 0; i >= 0; --i, ++ind) {
        char digit = ((unsigned int) (val / powl(10, i)) % 10);
        str[ind] = digit + 48;
        val -= digit * powl(10, i);
    }

    return str;
}


void concat(unsigned int count, char** str, const char* format, ...) {
    va_list args;
    va_start(args, format);

    unsigned int str_ind = 0;
    *str = (char*) realloc(*str, 150);

    for (unsigned int i = 0; i < count; ++i) {
        char* data;

        if (format[i] == 's') {
            data = va_arg(args, char*);
        } else {
            unsigned int val = va_arg(args, unsigned int);
            data = num_to_str(val);
        }

        for (unsigned int ind = 0; data[ind] != '\0'; ++ind, str_ind++) {
            (*str)[str_ind] = data[ind];
        }

        if (format[i] == 'u') {
            free(data);
        }

    }

    va_end(args);

    // Add the string terminator
    (*str)[str_ind] = '\0';
    *str = (char*) realloc(*str, str_ind + 1);

    return;
}

#endif // _UTILS_H_