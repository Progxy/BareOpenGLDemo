#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include <stdlib.h>

char* read_file(const char* file_path) {
    FILE* file = fopen(file_path, "rb");

    // Check file opener status
    if (file == NULL) {
        printf("OPEN_FILE:ERROR: an error occured while opening the file, check if the file exists or if the given path is correct!\n");
        return NULL;
    }

    // Get the length of the file
    fseek(file, 0, SEEK_END);
    unsigned int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Set the data buffer
    char* data = (char*) calloc(length, 1);
    length = fread(data, 1, length, file);

    // Check for errors
    int err = 0;
    if ((err = ferror(file))) {
        // Free memory previously allocated
        free(data);
        printf("READ_FILE:ERROR: the read operation terminated with the following error code: %d\n", err);
        return NULL;
    }

    // Clean the flags
    clearerr(file);

    // Close file
    fclose(file);

    return data;
}

#endif // _PARSER_H_
