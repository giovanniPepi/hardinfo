#include <stdio.h>

/* Reusable file opener for /proc files*/
FILE* open_file(const char* filename, const char* mode) {    
    
    FILE* file = fopen(filename, mode);
    if (file == NULL) {
        printf("Failed to open the file.\n");
    }
    return file;
}