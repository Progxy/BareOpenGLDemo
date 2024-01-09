#ifndef _UTILS_H_
#define _UTILS_H_

#define __USE_MISC 
#include <math.h>

#define TRUE 1
#define FALSE 0
#define WIDTH 800
#define HEIGHT 600
#define SENSITIVITY 0.1f
#define DEBUG_INFO(debug_str) printf("DEBUG: %s\n", debug_str)
#define CLIP(val, min, max) ((val > max) ? max : (val < min ? min : val))

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


#endif // _UTILS_H_