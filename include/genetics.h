#ifndef GENETICS_H
#define GENETICS_H

#include <stdlib.h>
#include <stdio.h>

/*
 * Genetic data is serialized with a 16-bit size counter
*/
#define GEN_MAX 65534

enum Type {
    TYPE_OPAQUE  = 0,
    TYPE_STRUCT  = 1,
    TYPE_POINTER = 2 /* unused */
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

void genetics_parse(FILE *file, struct TypeInfo *gen, size_t max_count);
void genetics_copy(void *dest, void *src, struct TypeInfo *dest_gen, struct TypeInfo *src_gen, const char *type);

void genetics_save(FILE *file, struct TypeInfo *gen);
void genetics_load(FILE *file, struct TypeInfo *gen, size_t max_count);
void genetics_print(struct TypeInfo *gen);

#endif

