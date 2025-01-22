#include "writer.h"

#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <stdarg.h>

int writer(char* file, char* string) {

    

    FILE* fp = fopen(file, "w");

    if (fp == NULL) {
        printf("could not open file: %s\n", strerror(errno));
        return -1;
    }

    fprintf(fp, "%s", string);

    return sizeof(string);
}

void log_and_print(int priority, char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    fflush(stdout);

    va_start(args, fmt);
    vsyslog(priority, fmt, args);
    va_end(args);
}

void *safe_malloc(size_t n)
{
    void *p = malloc(n);
    if (p == NULL) {
        log_and_print(LOG_ERR, "Fatal: failed to allocate %zu bytes.\n", n);
        abort();
    }
    return p;
}


int main(int argc, char **argv) {

    openlog("assignment02app", LOG_PID, LOG_USER);

    if (argc < 3) {
        log_and_print(LOG_ERR, "wrong number of arguments\n", NULL);
        return 1;
    }

    if (strcmp(argv[1], "") == 0 ) {
        log_and_print(LOG_ERR, "First parameter required, please specify a file.\n", NULL);
        return 1;
    }

    if (strcmp(argv[2], "") == 0 ) {
        log_and_print(LOG_ERR, "Second parameter required, please specify a string to write to file.\n", NULL);
        return 1;
    }

    char* file = safe_malloc(strlen(argv[1]) + 1);
    
    
    char* string = safe_malloc(strlen(argv[2]) + 1);


    strcpy(file, argv[1]);
    strcpy(string, argv[2]);

    log_and_print(LOG_INFO,"Writing <%s> to <%s>\n", string, file);

    writer(file, string);



    free(file);
    free(string);

    closelog();
    
    return 0;
}