#ifndef _UTILS_H_
#define _UTILS_H_

#define TRUE 1
#define FALSE 0

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