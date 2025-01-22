#include <stdio.h>
#include <errno.h>
int main(int argc, char **argv);
int writer(char* file, char* string);
void *safe_malloc(size_t);
void log_and_print(int, char*, ...);