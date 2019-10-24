#ifndef STRUCTS_H
#define STRUCTS_H

/* version A */
struct Bar_A {
    float x;
    int z[8];
};

struct Foo_A {
    int a;
    float b;
    struct Bar_A bar;
};

/* version B */
struct Bar_B {
    int z[8];
    float x;
};

struct Foo_B {
    struct Bar_B bar;
    float b;
    float c;
    int a;
};

#endif
