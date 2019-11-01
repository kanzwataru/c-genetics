#ifndef STRUCTS_H
#define STRUCTS_H

/* version A */
struct Bar {
    float x;
    int z[8];
};

struct Foo {
    int a;
    float b;
    struct Bar bar;
};

#endif
