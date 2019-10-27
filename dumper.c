#include <stdio.h>
#include <stddef.h>
#include "genetics.h"
#include "dumper.gen.h"

int main(int argc, char **argv) {
    FILE *fp = fopen("dumped.dat", "wb");
    genetics_save(fp, genetic);
    fclose(fp);
}

