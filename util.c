#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "genetics.h"

#define GEN_MAX 65535
struct TypeInfo genetic[GEN_MAX];

void help(void) {
    fprintf(stderr, "util <genetics file> <operation>\n");
    fprintf(stderr, "\noperations:\n");
    fprintf(stderr, "\tprint: Load and print the contents\n");
}

int main(int argc, char **argv) {
    FILE *f;

    if(argc < 3) {
        help();
        return 1;
    }

    f = fopen(argv[1], "rb");
    if(!f) {
        fprintf(stderr, "cannot open %s\n", argv[1]);
        return 2;
    }

    if(0 == strcmp(argv[2], "print")) {
        genetics_load(f, genetic, GEN_MAX);
        genetics_print(genetic);
    }
    else {
        help();
    }

    fclose(f);
    return 0;
}

