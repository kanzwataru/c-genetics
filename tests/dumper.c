#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include "genetics.h"
#include "dumper.gen.h"

int main(int argc, char **argv) {
    assert(argc == 2);
    FILE *fp = fopen(argv[1], "wb");
    assert(fp);
    genetics_save(fp, genetic);
    fclose(fp);
}

