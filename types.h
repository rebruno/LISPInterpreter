#ifndef LISPTYPES
#define LISPTYPES

#define true 1
#define false 0

typedef enum {EMPTY_LIST, SYMBOL, FIXNUM, BOOLEAN, CHARACTER, STRING, PAIR, PRIMITIVE, COMPOUND, ERROR } otype;
const char *enum_strings[] = 
    {"EMPTY_LIST", "SYMBOL", "FIXNUM", "BOOLEAN", "CHARACTER", "STRING", "PAIR", "PRIMITIVE", "COMPOUND"};

typedef struct object {
    //otype type;
    short type;
    unsigned short flags; //Used for GC and other stuff
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

        //Consider adding an arity field
        struct {
            struct object *car;
            struct object *cdr;
        } pair;

        struct {
            struct object* params;
            struct object* body;
            struct object* env;
        } compound;

        struct{
            struct object* (*function)(struct object* args);
        } primitive;

    } data;

} object;





extern object* empty_list;
extern object* otrue;
extern object* ofalse;

extern object* quote_symbol;
extern object* set_symbol;
extern object* definition_symbol;
extern object* if_symbol;
extern object* else_symbol;
extern object* lambda_symbol;
extern object* begin_symbol;
extern object* cond_symbol;
extern object* exit_symbol;


#endif