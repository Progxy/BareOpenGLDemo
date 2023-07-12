#ifndef _PARSER_H_
#define _PARSER_H_

char* readFile(const char* file_path) {
    FILE* file = fopen(file_path, "r");

    // Check file opener status
    if (file == NULL) {
        printf("OPEN_FILE:ERROR: an error occured while opening the file, check if the file exists or if the given path is correct!\n");
        return NULL;
    } 

    // Init variables to store the data and eventual flags results
    char* data = calloc(1, 1);
    int len, err, eof;

    // Clean the flags
    clearerr(file);

    // Read the data and check for errors
    for (len = 0, err = 0, eof = 0; !(eof = feof(file)) && !(err = ferror(file)); fread(data + len, 1, 1, file), len++) {
        data = realloc(data, len + 1);
    }

    data[len] = 0;

    // Clean the flags
    clearerr(file);

    // Check for errors 
    if (err) {
        // Free memory previously allocated
        free(data);
        printf("READ_FILE:ERROR: the read operation terminated with the following error code: %d\n", err);
        return NULL;
    }

    // Close file
    fclose(file);

    return data;
}

#endif // _PARSER_H_