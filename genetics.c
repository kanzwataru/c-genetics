#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "genetics.h"

static struct TypeInfo *find_type(const char *struct_type, const char *type, struct TypeInfo *gen)
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

void genetics_copy(void *dest,
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
            genetics_copy((char *)dest + dest_ti->offset,
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

void genetics_print(struct TypeInfo *gen)
{
    printf("\n*** genetics printout for TypeInfo at %p ***\n", gen);
    while(gen->struct_name != 0) {
        printf("in %s -> %d '%s' of type '%s' with size %d at offset %d ",
                gen->struct_name, gen->count, gen->member_name, gen->type_name, gen->size, gen->offset);
        switch(gen->type) {
            case TYPE_OPAQUE:
                printf("(opaque type)\n");
                break;
            case TYPE_STRUCT:
                printf("(nested struct type)\n");
                break;
            case TYPE_POINTER:
                printf("(pointer type)\n");
                break;
            default:
                printf("(unknown type!)\n");
                break;
        }

        gen++;
    }
}

void genetics_save(FILE *file, struct TypeInfo *gen)
{
    struct TypeInfo *p = gen;
    size_t count = 0, i = 0;
    unsigned short count_out;
    unsigned int out32;

    while(p->struct_name != 0) { ++count; ++p; }

    /* temp assertions */
    assert(sizeof(unsigned short) == 2);
    assert(sizeof(unsigned int) == 4);
    assert(count < 65534);

    count_out = count;
    fwrite(&count_out, sizeof(unsigned short), 1, file);
    for(i = 0; i < count; ++i) {
        assert(gen[i].struct_name);
        assert(gen[i].member_name);
        assert(gen[i].type_name);

    #define output_string(_str) \
        out32 = strlen(_str); \
        fwrite(&out32, sizeof(unsigned int), 1, file); \
        fputs(_str, file);

        output_string(gen[i].struct_name);
        output_string(gen[i].member_name);
        output_string(gen[i].type_name);
        
        out32 = gen[i].size;
        fwrite(&out32, sizeof(unsigned int), 1, file);
        
        out32 = gen[i].count;
        fwrite(&out32, sizeof(unsigned int), 1, file);
        
        out32 = gen[i].offset;
        fwrite(&out32, sizeof(unsigned int), 1, file);

        fputc(gen[i].type, file);
    }
}

void genetics_load(FILE *file, struct TypeInfo *gen, size_t max_count)
{
    unsigned short count;
    unsigned int in32;
    size_t i;

    memset(gen, 0, max_count);
    
    fread(&count, sizeof(count), 1, file);

    /* temp assertions */
    assert(count < max_count - 1);
    assert(sizeof(unsigned short) == 2);
    assert(sizeof(unsigned int) == 4);

    for(i = 0; i < count; ++i) {
    #define read_string(_str) \
        fread(&in32, sizeof(in32), 1, file); \
        _str = calloc(in32 + 1, 1); \
        fread(_str, 1, in32, file);

        read_string(gen[i].struct_name);
        read_string(gen[i].member_name);
        read_string(gen[i].type_name);

    #define read_size(_str) \
        fread(&in32, sizeof(in32), 1, file); \
        _str = in32;

        read_size(gen[i].size);
        read_size(gen[i].count);
        read_size(gen[i].offset);
        
        gen[i].type = fgetc(file);
    }
}
