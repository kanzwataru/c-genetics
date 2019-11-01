#include "structs.h"
#include "genetics.h"
#include "structs_genetics.gen.h"
#include <stdio.h>
#include <assert.h>

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

static void print_bar(struct Bar *bar, int indents)
{
    int i, z;

    #define INDENT() for(z = 0; z < indents; ++i) { printf("\t"); }

    INDENT(); printf("start Bar\n");
    for(i = 0; i < 8; ++i) {
        INDENT(); printf("\tz[%d]: %d\n", bar->z[i]);
    }
    INDENT(); printf("\tx: %f\n", bar->x);
    INDENT(); printf("end Bar"\n);

    #undef INDENT
}

static void print_foo(struct Foo *foo)
{
    fprintf("start Foo\n");
    print_bar(foo->bar, 1);
    fprintf("\tb: %f\n", foo->b);
    fprintf("\tc: %f\n", foo->c);
    fprintf("\ta: %d\n", foo->a);
    fprintf("end Foo\n\n");
}

int main(int argc, char **argv)
{
    int ret;
    FILE *fp;

    /* write out own genetics */
    fp = fopen("prog_b_gen.bin", "wb");
    assert(fp);
    genetics_save(fp, structs_gen);
    fclose(fp);

    /* write out own data */
    fp = fopen("prog_b_data.bin", "wb");
    assert(fp);
    ret = fwrite(&f, sizeof(f), 1, fp); assert(ret == 1);
    ret = fwrite(&b, sizeof(b), 1, fp); assert(ret == 1);
    fclose(fp);

    /* print own data */
    print_foo(&f);
    print_bar(&b);

    return 0;
}

