#include "writer.h"

#include <stdlib.h>
#include <string.h>



int writer(char* file, char* string) {



    FILE* fp = fopen(file, "w");

    if (fp == NULL) {
        printf("could not open file: %s\n", strerror(errno));
        return -1;
    }

    fprintf(fp, "%s", string);

    return sizeof(string);
}

int main(int argc, char *argv[]) {



    if (argc < 2) {
        printf("wrong number of arguments\n");
        return 1;
    } else {
        if (strcmp(argv[1], "") == 0 ) {
            printf("First parameter required, please specify a file.\n");
            return 1;
        }

        if (strcmp(argv[2], "") == 0 ) {
            printf("Second parameter required, please specify a string to write to file.\n");
            return 1;
        }
    }

    char* file = malloc(strlen(argv[1]) + 1);
    char* string = malloc(strlen(argv[2]) + 1);
    strcpy(file, argv[1]);
    strcpy(string, argv[2]);


    int chars_written = writer(file, string);



    free(file);
    free(string);

    return 0;
}