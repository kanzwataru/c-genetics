#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "genetics.h"
#include "structs.h"

struct TypeInfo genetics_a[] = {
     {"Bar", "x", "float", sizeof(float), 1, offsetof(struct Bar_A, x), TYPE_OPAQUE},
     {"Bar", "z", "int", sizeof(int), 8, offsetof(struct Bar_A, z), TYPE_OPAQUE},

     {"Foo", "a", "int", sizeof(int), 1, offsetof(struct Foo_A, a), TYPE_OPAQUE},
     {"Foo", "b", "float", sizeof(float), 1, offsetof(struct Foo_A, b), TYPE_OPAQUE},
     {"Foo", "bar", "Bar", sizeof(struct Bar_A), 1, offsetof(struct Foo_A, bar), TYPE_STRUCT},
     {0}
};

struct TypeInfo genetics_b[] = {
     {"Bar", "z", "int", sizeof(int), 8, offsetof(struct Bar_B, z), TYPE_OPAQUE},
     {"Bar", "x", "float", sizeof(float), 1, offsetof(struct Bar_B, x), TYPE_OPAQUE},

     {"Foo", "bar", "Bar", sizeof(struct Bar_A), 1, offsetof(struct Foo_B, bar), TYPE_STRUCT},
     {"Foo", "b", "float", sizeof(float), 1, offsetof(struct Foo_B, b), TYPE_OPAQUE},
     {"Foo", "c", "float", sizeof(float), 1, offsetof(struct Foo_B, c), TYPE_OPAQUE}, 
     {"Foo", "a", "int", sizeof(int), 1, offsetof(struct Foo_B, a), TYPE_OPAQUE},
     {0}
};

#define print_foo_struct(_var) \
    printf("\n**%s**\n", #_var); \
    printf("a: %d\n", _var.a); \
    printf("b: %f\n", _var.b); \
    printf("bar.x: %f\n", _var.bar.x); \
    for(int i = 0; i < 8; ++i) { \
        printf("bar.z[%d]: %d\n", i, _var.bar.z[i]); \
    }

int main(void)
{
    struct Foo_A a = {
        42,
        1.337f,
        {
            -100.55f,
            {10, 20, 30, 40, 50, 60, 70, 80}
        },
    };

    struct Foo_B b;
    struct Foo_A c;
    genetics_copy(&b, &a, genetics_b, genetics_a, "Foo");
    genetics_copy(&c, &a, genetics_a, genetics_a, "Foo");

    genetics_print(genetics_a);
    //genetics_print(genetics_b);

    print_foo_struct(a);
    print_foo_struct(b);
    printf("c: %f\n", b.c);

    print_foo_struct(c);

    return 0;
}

