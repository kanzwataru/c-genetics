#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "genetics.h"

#define GEN_MAX 65534
struct TypeInfo genetic[GEN_MAX];

char *strdcat(const char *a, const char *b) {
    size_t a_len;
    size_t b_len;
    char *c;

    a_len = strlen(a);
    b_len = strlen(b);
    c = malloc(a_len + b_len + 1);

    memcpy(c, a, a_len);
    memcpy(c + a_len, b, b_len);
    c[a_len + b_len] = '\0';

    return c; 
}

void strdcat_m(char **a, const char *b) {
    char *c = strdcat(*a, b);

    free(*a);
    *a = c;
}

char *strditoa(int value)
{
    size_t count;
    char *buf;
            
    count = snprintf(0, 0, "%d", value) + 1;
    buf = calloc(count, 1);
    snprintf(buf, count, "%d", value);

    return buf;
}

void make_dumper_header_file(FILE *f, struct TypeInfo *g, const char *header_name) {
    size_t i;
    char *t;
    char *tag;

    fprintf(f, "#ifndef GENETIC_DUMP_GEN_H\n");
    fprintf(f, "#define GENETIC_DUMP_GEN_H\n\n");
    fprintf(f, "#include \"%s\"\n\n", header_name);

    fprintf(f, "static struct TypeInfo genetic[] = {\n");
    for(i = 0; g[i].struct_name != 0; ++i) {
        switch(g[i].type) {
        case TYPE_OPAQUE:
            t = "TYPE_OPAQUE";
            tag = "";
            break;
        case TYPE_STRUCT:
            t = "TYPE_STRUCT";
            tag = "struct ";
            break;
        case TYPE_POINTER:  /* not implemented */
        default:
            assert(0);
            break;
        }

        fprintf(f, "\t{\"%s\", \"%s\", \"%s\", sizeof(%s%s), %d, offsetof(struct %s, %s), %s},\n",
                g[i].struct_name, g[i].member_name, g[i].type_name,
                tag, g[i].type_name, g[i].count, g[i].struct_name, g[i].member_name, t);
    }
    fprintf(f, "};\n");

    fprintf(f, "\n#endif\n");
}

char *make_dumper_header(struct TypeInfo *g, const char *header_name) {
    size_t i;
    char *num;
    char *header = strdcat("#include \"", header_name);
    strdcat_m(&header, "\"\n");
    strdcat_m(&header, "struct TypeInfo genetic[] = {\n");

    for(i = 0; g[i].struct_name != 0; ++i) {
        strdcat_m(&header, "\t{");

        strdcat_m(&header, g[i].struct_name);
        strdcat_m(&header, ", ");

        strdcat_m(&header, g[i].member_name);
        strdcat_m(&header, ", ");
        strdcat_m(&header, g[i].type_name);
        strdcat_m(&header, ", ");

        strdcat_m(&header, "sizeof(");
        strdcat_m(&header, g[i].type_name);
        strdcat_m(&header, "), ");

        num = strditoa(g[i].count);
        strdcat_m(&header, num);
        strdcat_m(&header, ", ");
        free(num);
        
        strdcat_m(&header, "offsetof(struct ");
        strdcat_m(&header, g[i].struct_name);
        strdcat_m(&header, "), ");

        switch(g[i].type) {
            case TYPE_OPAQUE:
                strdcat_m(&header, "TYPE_OPAQUE");
                break;
            case TYPE_STRUCT:
                strdcat_m(&header, "TYPE_STRUCT");
                break;
            case TYPE_POINTER: /* not implemented */
            default:
                assert(0);
                break;
        }

        strdcat_m(&header, "},\n");
    }

    strdcat_m(&header, "};\n");

    return header;
}

int main(int argc, char **argv) {
    FILE *in;    

    if(argc != 3) {
        puts("make_dumper <header file> <genetics data>");
        return 1;
    }

    in = fopen(argv[2], "rb");
    genetics_load(in, genetic, GEN_MAX);
    fclose(in);

    /*
    char *header = make_dumper_header(genetic, argv[1]);
    fprintf(stdout, "%s\n", header);
    free(header);
    */

    make_dumper_header_file(stdout, genetic, argv[1]);

    return 0;
}

