#include "structs.h"
#include "genetics.h"
#include <stdio.h>
#include <assert.h>

static struct Foo f = {
    42,
    -32.4f,
    {
        15.5f,
        {80, 70, 60, 50, 40, 30, 20, 10}
    }
};

static struct Bar b = {
    0.1337f,
    {10, 20, 30, 40, 50, 60, 70, 80}
};

static struct TypeInfo structs_gen[GEN_MAX];

static void print_bar(struct Bar *bar, int indents)
{
    int i, z;

    #define INDENT() for(z = 0; z < indents; ++z) { printf("\t"); }

    INDENT(); printf("start Bar\n");
    INDENT(); printf("\tx: %f\n", bar->x);
    for(i = 0; i < 8; ++i) {
        INDENT(); printf("\tz[%d]: %d\n", i, bar->z[i]);
    }
    INDENT(); printf("end Bar\n");

    #undef INDENT
}

static void print_foo(struct Foo *foo)
{
    printf("start Foo\n");
    printf("\ta: %d\n", foo->a);
    printf("\tb: %f\n", foo->b);
    print_bar(&foo->bar, 1);
    printf("end Foo\n\n");
}

int main(int argc, char **argv)
{
    int ret;
    FILE *fp;

    /* get our own genetics */
    fp = fopen("build/prog_a_genetics.bin", "r");
    assert(fp);
    genetics_load(fp, structs_gen, GEN_MAX);
    fclose(fp);

    /* write out own data */
    fp = fopen("prog_a_data.bin", "wb");
    assert(fp);
    ret = fwrite(&f, sizeof(f), 1, fp); assert(ret == 1);
    ret = fwrite(&b, sizeof(b), 1, fp); assert(ret == 1);
    fclose(fp);

    /* print own data */
    print_foo(&f);
    print_bar(&b, 0);

    return 0;
}

