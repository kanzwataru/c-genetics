#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "genetics.h"

enum Token {
    TOK_NONE,
    TOK_STRUCT,
    TOK_NAME,
    TOK_OPEN_BRACE,
    TOK_CLOSE_BRACE,
    TOK_OPEN_SQUARE,
    TOK_CLOSE_SQUARE,
    TOK_SEMICOLON,
    TOK_WHITESPACE,
    TOK_UNKNOWN
};

enum State {
    ST_NONE,
    ST_STRUCT_PREFIX,
    ST_STRUCT_NAME,
    ST_TYPE_PREFIX,
    ST_IN_STRUCT,
    ST_IN_COMMENT,
    ST_IN_ARRAYNUM
};

static int c;
static int last = 0;
static int in_comment = 0;
static int in_line_comment = 0;

#define TOK_MAX 512
static char string[TOK_MAX];
static size_t str_top = 0;

static char *last_struct = NULL;
static char *current_struct = NULL;

static enum Token last_token = TOK_NONE;
static enum Token current_token = TOK_NONE;
static enum State state = ST_NONE;

static void clear_string() {
    memset(string, 0, TOK_MAX);
    str_top = 0;
}

static void set_token(enum Token tok) {
    if(current_token != tok) {
        last_token = current_token;
        current_token = tok;
    }
}

void genetics_parse(FILE *file, struct TypeInfo *gen, size_t max_count) {
    size_t gen_top = 0;

    assert(max_count <= GEN_MAX);

    while(EOF != (c = getc(file))) {
        switch(c) {
        case '/':
            if(last == '*' && in_comment)
                in_comment = 0;

            if(last == '/')
                in_line_comment = 1;
            
            last = c;
            break;
        case '*':
            if(last == '/')
                in_comment = 1;
            
            last = c;
            break;
        case '#':
            in_line_comment = 1;
            break;
        case '\n':
            if(in_line_comment)
                in_line_comment = 0;
            break;
        case '{':
            set_token(TOK_OPEN_BRACE);
            break;
        case '}':
            set_token(TOK_CLOSE_BRACE);
            clear_string();
            break;
        case '[':
            set_token(TOK_OPEN_SQUARE);
            break;
        case ']':
            set_token(TOK_CLOSE_SQUARE);
            break;
        case ' ':
            set_token(TOK_WHITESPACE);
            break;
        case ';':
            set_token(TOK_SEMICOLON);
            ////printf("SEMICOLON\n");
            break;
        default:
            if(!in_comment && !in_line_comment) {
                //f//printf(stderr,"%c", c);
            
                assert(str_top + 1 < TOK_MAX - 1);
                string[str_top++] = c;
                if(0 == strcmp("struct", string)) {
                    set_token(TOK_STRUCT);
                }
                else {
                    set_token(TOK_NAME);
                }
            }
            break; 
        }

        switch(current_token) {
            case TOK_STRUCT:
                //printf("expecting struct name...\n");
                clear_string();
                break;
            case TOK_WHITESPACE:
                if(last_token == TOK_STRUCT) {
                    state = ST_STRUCT_PREFIX;
                }
                if(last_token == TOK_NAME && state == ST_STRUCT_PREFIX) {
                    state = ST_STRUCT_NAME;
                    //printf("got struct named: %s\n", string);
                    
                    last_struct = current_struct;
                    current_struct = strdup(string);
                    
                    clear_string();
                }
                if(last_token == TOK_NAME && state == ST_IN_STRUCT) {
                    state = ST_TYPE_PREFIX;
                    //printf("    type: %s ", string);
                    last_token = TOK_NONE;

                    gen[gen_top].struct_name = current_struct;
                    gen[gen_top].type_name = strdup(string);

                    clear_string();
                }
                else {
                }

                break;
            case TOK_SEMICOLON:
                if(last_token == TOK_NAME && state == ST_TYPE_PREFIX) {
                    //printf("name: %s END VAR\n", string);
                    state = ST_IN_STRUCT;
                    last_token = TOK_NONE;

                    gen[gen_top].count = 1;
                    gen[gen_top].member_name = strdup(string);
                    gen[gen_top].type = TYPE_OPAQUE;
                    ++gen_top;
                    assert(gen_top < max_count);

                    clear_string();
                }
                if(last_token == TOK_NAME && state == ST_STRUCT_NAME) {
                    //printf("    -> name: %s END NESTED STRUCT\n", string);
                    state = ST_IN_STRUCT;
                    last_token = TOK_NONE;

                    gen[gen_top].count = 1;
                    gen[gen_top].struct_name = last_struct;
                    gen[gen_top].type_name = current_struct;
                    gen[gen_top].member_name = strdup(string);
                    gen[gen_top].type = TYPE_STRUCT;
                    ++gen_top;
                    assert(gen_top < max_count);

                    current_struct = last_struct;
                    clear_string();
                }
                else {
                }

                break;
            case TOK_OPEN_BRACE:
                if(state == ST_STRUCT_NAME) {
                    //printf("BEGIN STRUCT\n");
                    state = ST_IN_STRUCT;
                }
                else {
                }

                break;
            case TOK_CLOSE_BRACE:
                if(state == ST_IN_STRUCT) {
                    //printf("END STRUCT\n\n");
                    state = ST_NONE;
                }
                else {
                }

                break;
            case TOK_OPEN_SQUARE:
                if(last_token == TOK_NAME && state == ST_TYPE_PREFIX) {
                    //printf("name: %s ", string);
                    state = ST_IN_ARRAYNUM;

                    gen[gen_top].member_name = strdup(string);

                    clear_string();
                }
                else {
                }

                break;
            case TOK_CLOSE_SQUARE:
                if(last_token == TOK_NAME && state == ST_IN_ARRAYNUM) {
                    //printf("count: %s END ARRAY\n", string);
                    state = ST_IN_STRUCT;
                    last_token = TOK_NONE;

                    gen[gen_top].count = atoi(string);
                    gen[gen_top].type = TYPE_OPAQUE;
                    gen_top++;
                    assert(gen_top < max_count);

                    clear_string();
                }
                else {
                }

                break;
        }
    }
}

/*
void test_genetics_readwrite(void) {
    int i, count_in = 0, count_out = 0;
    struct TypeInfo gen[256];
    const char *out_filename = "genetics_out.dat";
    FILE *tmp = fopen(out_filename, "wb");
    genetics_save(tmp, genetic);
    fclose(tmp);
    fopen(out_filename, "rb");
    genetics_load(tmp, gen, 256);
    //genetics_print(gen);
    fclose(tmp);

    while(gen[count_out].struct_name != 0) { ++count_out; }
    while(gen[count_in].struct_name != 0) { ++count_in; }
    
    assert(count_out == count_in);
    for(i = 0; i < count_in; ++i) {
    #define compare_string(_str) \
        assert(0 == strcmp(gen[i]._str, gen[i]._str));
    #define compare_vals(_val) \
        assert(gen[i]._val == gen[i]._val);

        compare_string(struct_name);
        compare_string(member_name);
        compare_string(type_name);
        compare_vals(size);
        compare_vals(count);
        compare_vals(offset);
        compare_vals(type);
    
    #undef compare_string
    #undef compare_vals
    }

    printf("Successful read/write of genetics data to/from %s\n", out_filename);
}
*/
