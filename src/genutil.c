#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "genetics.h"

struct TypeInfo genetic[GEN_MAX];

void help(void) {
    fprintf(stderr, "genutil print <genetics file>\n");
    fprintf(stderr, "genutil parse <header file> <genetics file>\n");
}

int main(int argc, char **argv) {
    FILE *f, *h;

    if(argc < 3) {
        help();
        return 1;
    }

    if(0 == strcmp(argv[1], "print")) {
        f = fopen(argv[2], "rb");
        if(!f) {
            fprintf(stderr, "cannot open %s\n", argv[2]);
            return 2;
        }

        genetics_load(f, genetic, GEN_MAX);
        genetics_print(genetic);
    
        fclose(f);
    }
    else if(0 == strcmp(argv[1], "parse")) {
        if(argc < 4) {
            help();
            return 1;
        }

        h = fopen(argv[2], "rb");
        if(!h) {
            fprintf(stderr, "cannot open %s\n", argv[2]);
            return 2;
        }

        f = fopen(argv[3], "wb");
        if(!f) {
            fprintf(stderr, "cannot open %s for writing\n", argv[3]);
            fclose(h);
            return 3;
        }

        genetics_parse(h, genetic, GEN_MAX);
        genetics_save(f, genetic);

        fclose(f);
        fclose(h);
    }
    else {
        help();
    }

    return 0;
}

