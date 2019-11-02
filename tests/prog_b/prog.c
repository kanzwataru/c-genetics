#include "structs.h"
#include "genetics.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

static struct Foo f = {
    {
        {88, 77, 66, 55, 44, 33, 22, 11},
        -1337.15f
    },
    -10.42f,
    0.5f,
    0xDEADBEEF
};

static struct Bar b = {
    {11, 22, 33, 44, 55, 66, 77, 88},
    15.1337f
};

static struct TypeInfo structs_gen[GEN_MAX];
static struct TypeInfo foreign_gen[GEN_MAX];

static void print_bar(struct Bar *bar, int indents)
{
    int i, z;

    #define INDENT() for(z = 0; z < indents; ++z) { printf("\t"); }

    INDENT(); printf("start Bar\n");
    for(i = 0; i < 8; ++i) {
        INDENT(); printf("\tz[%d]: %d\n", i, bar->z[i]);
    }
    INDENT(); printf("\tx: %f\n", bar->x);
    INDENT(); printf("end Bar\n");

    #undef INDENT
}

static void print_foo(struct Foo *foo)
{
    printf("start Foo\n");
    print_bar(&foo->bar, 1);
    printf("\tb: %f\n", foo->b);
    printf("\tc: %f\n", foo->c);
    printf("\ta: %d\n", foo->a);
    printf("end Foo\n\n");
}

void help(void)
{
    puts("prog_b <out>");
    puts("prog_b <compare> <in genetics> <in data>");
}

int main(int argc, char **argv)
{
    int ret;
    FILE *fp;

    size_t size = 0;
    void *data;
    struct Foo foreign_foo;
    struct Bar foreign_bar;

    if(argc < 2) {
        help();
        return 1;
    }

    /* get our own genetics */
    fp = fopen("build/prog_b_genetics.bin", "r");
    assert(fp);
    genetics_load(fp, structs_gen, GEN_MAX);
    fclose(fp);

    if(0 == strcmp("out", argv[1])) {
        /* write out own data */
        fp = fopen("prog_b_data.bin", "wb");
        assert(fp);
        ret = fwrite(&f, sizeof(f), 1, fp); assert(ret == 1);
        ret = fwrite(&b, sizeof(b), 1, fp); assert(ret == 1);
        fclose(fp);

        /* print own data */
        print_foo(&f);
        print_bar(&b, 0);
    }
    else if(0 == strcmp("compare", argv[1])) {
        if(argc != 4) {
            help();
            return 1;
        }

        /* read the provided genetics */
        fp = fopen(argv[2], "rb");
        assert(fp);
        genetics_load(fp, foreign_gen, GEN_MAX);
        fclose(fp);

        /* read the data */
        fp = fopen(argv[3], "rb");
        assert(fp);
        while(getc(fp) != EOF) ++size;
        rewind(fp);

        data = calloc(size, 1);
        assert(data);
        ret = fread(data, 1, size, fp); assert(ret == size);

        genetics_copy(&foreign_foo, data, structs_gen, foreign_gen, "Foo");

        /* print the data */
        print_foo(&foreign_foo);
    }
    else {
        help();
        return 1;
    }

    return 0;
}

