#ifndef LISPTYPES
#define LISPTYPES

#define true 1
#define false 0

typedef enum {EMPTY_LIST, SYMBOL, FIXNUM, BOOLEAN, CHARACTER, STRING, PAIR, PRIMITIVE, COMPOUND } otype;
const char *enum_strings[] = 
    {"EMPTY_LIST", "SYMBOL", "FIXNUM", "BOOLEAN", "CHARACTER", "STRING", "PAIR", "PRIMITIVE", "COMPOUND"};

typedef struct object {
    otype type;
    union{
        struct {
            int value;
        } fixnum;

        struct {
            unsigned int value;
        } boolean;

        struct {
            char* value;
        } symbol;
        struct {
            char value;
        } character;

        struct String {
            char* value;
        } string;

        struct {
            struct object *car;
            struct object *cdr;
        } pair;

    } data;

} object;





extern object* empty_list;
extern object* otrue;
extern object* ofalse;
extern object* quote_symbol;




#endif