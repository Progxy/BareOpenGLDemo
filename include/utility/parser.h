#ifndef _PARSER_H_
#define _PARSER_H_

//#ifdef _PARSER_IMPLEMENTATION_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* readFile(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    char* data = calloc(1, 1);
    int len, err, eof;

    // Read the data and check for errors
    for (len = 0, err = 0, eof = 0; !(eof = feof(file)) || !(err = ferror(file)); fread(data + len, 1, 1, file), len++) {
        data = realloc(data, len + 1);
    }

    // Check for errors 
    if (err) {
        free(data);
        char err_log[80] = "READ_FILE:ERROR: the read operation terminated with the following error code:  ";
        return err_log;
    }

    return data;
}

//#endif

char* readFile(const char* file_path);

#endif // _PARSER_H_