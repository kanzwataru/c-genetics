#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

/* genetics */
enum Type {
    TYPE_OPAQUE,
    TYPE_STRUCT,
    TYPE_POINTER /* unused */
};

struct TypeInfo {
    char *struct_name;
    char *member_name;
    char *type_name;
    size_t size;
    size_t count;
    size_t offset;
    enum Type type;
};

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

struct TypeInfo *find_type(const char *struct_type, const char *type, struct TypeInfo *gen)
{
    while(gen->struct_name != 0) {
        if(0 == strcmp(struct_type, gen->struct_name)) {
            if(type) { /* are we searching for a member type as well ?*/
                if(0 == strcmp(type, gen->member_name)) {
                    return gen;
                }
            }
            else {    /* we just want something that matches the struct */
                return gen;
            }
        }
        
        ++gen;
    }

    return NULL;
}

void genetic_copy(void *dest,
                  void *src, 
                  struct TypeInfo *dest_gen,
                  struct TypeInfo *src_gen,
                  const char *type)
{
    struct TypeInfo *dest_ti = dest_gen, *src_ti;

    while((dest_ti = find_type(type, NULL, dest_ti))) {
        src_ti = find_type(type, dest_ti->member_name, src_gen);
        if(!src_ti) {
            printf("Skipped %s of %s\n", dest_ti->member_name, dest_ti->struct_name);
            goto next;
        }

        switch(dest_ti->type) {
        case TYPE_OPAQUE:
            assert(dest_ti->type == src_ti->type);
            assert(dest_ti->size == src_ti->size);
            assert(dest_ti->count == src_ti->count);

            memcpy((char *)dest + dest_ti->offset,
                   (char *)src + src_ti->offset,
                   dest_ti->count * dest_ti->size);

            printf("Copied %s of %s\n", dest_ti->member_name, dest_ti->struct_name);

            break;
        case TYPE_STRUCT:
            genetic_copy((char *)dest + dest_ti->offset,
                         (char *)src + src_ti->offset,
                         dest_gen,
                         src_gen,
                         dest_ti->type_name);
            break;
        case TYPE_POINTER:
            break; /* not implemented */
        default:
            assert(0);
            break;
        }

    next:
        ++dest_ti;
    }
}

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
    genetic_copy(&b, &a, genetics_b, genetics_a, "Foo");
    genetic_copy(&c, &a, genetics_a, genetics_a, "Foo");

    print_foo_struct(a);
    print_foo_struct(b);
    printf("c: %f\n", b.c);

    print_foo_struct(c);

    return 0;
}

