#ifndef STRUCTS_H
#define STRUCTS_H

/* version B */
struct Bar {
    int z[8];
    float x;
};

struct Foo {
    struct Bar bar;
    float b;
    float c;
    int a;
};

#endif
