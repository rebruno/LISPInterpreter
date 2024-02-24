//utils.h contains utility functions like allocation and others

#ifndef LISPUTILS
#define LISPUTILS

#include "types.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

void error(char* s){
    printf("%s\n", s);
}

void *allocate(size_t size){ //not GC'd, 
    //return malloc(size);
    return calloc(1, size);
}

object *create_object(){
    object *obj;

    obj = allocate(sizeof *obj);
    if (obj == NULL){
        //Could add a garbage collector here
        error("Ran out of memory");
        exit(1);
    }


    return obj;
}



object* error_object(char* s){
    object* o = create_object();
    o->type = ERROR;
    o->data.string.value = s;
    return o;
}

void warning(char* s){
    printf("%s\n", s);
}

void exit_proc(int n){
    exit(n);
}

object* primitive_exit(object* arg){

    exit(arg->data.fixnum.value);
    return 0L;
}




int is_empty_list(object* list){
    return list == empty_list;
}

int is_pair(object* pair){
    return pair->type == PAIR;
}

int is_symbol(object* symbol){
    return symbol->type == SYMBOL;
}

int is_fixnum(object* fixnum){
    return fixnum->type == FIXNUM;
}

object* cons(object *car, object *cdr){
    object *ocons = create_object();
    ocons->type = PAIR;
    
    ocons->data.pair.car = car;
    ocons->data.pair.cdr = cdr;

    return ocons;
}

object* car(object* pair){
    if (is_pair(pair)){
        return pair->data.pair.car;
    }
    error("Expected a pair when using car!");
    return empty_list;
}

object* cdr(object* pair){
    if (is_pair(pair)){
        return pair->data.pair.cdr;
    }
    error("Expected a pair when using cdr!");
    return empty_list;
}

void set_car(object* o, object* val){
    o->data.pair.car = val;
}


void set_cdr(object* o, object* val){
    o->data.pair.cdr = val;
}

int is_true(object* o){
    return o == otrue;
}

int is_false(object* o){
    return o == ofalse;
}

int is_assignment(object* o){
    return car(o) == set_symbol;
}

int is_quoted(object* o){
    return car(o) == quote_symbol;
}

int is_definition(object* o){
    return car(o) == definition_symbol;
}

int is_if(object* o){
    return car(o) == if_symbol;
}

int is_lambda(object* o){
    return car(o) == lambda_symbol;
}

int is_begin(object* o){
    return car(o) == begin_symbol;
}

int is_cond(object* o){
    return car(o) == cond_symbol;
}

int is_self_eval(object* o){
    return o->type == FIXNUM || o->type == BOOLEAN ||o->type == CHARACTER 
            || o->type == STRING;
}

int is_application(object* o){
    return is_pair(o);
}

int eq(object* o1, object* o2){
    return o1 == o2;
}

int is_exit(object* o){
    return o == exit_symbol;
}

int list_length(object* lst){
    
    int c = 0;

    while (!is_empty_list(lst)){
        if (!is_pair(lst)){
            error("Expected a pair when using list_length");
            return -1;
        }
        c++;
        lst = cdr(lst);
    }

    return c;
}

int arity_check(object* args, int arity){
    /*
    arity is the number of arguments
    for variable arity, the minimum number of arguments is the negative
    so for the '+' primitive, it needs at least 2 arguments, so it has an arity of -2
    */
    if (arity < 0){
        return list_length(args) > -arity;
    }
    return list_length(args) == arity;
}

object* make_fixnum(int value){
    object* o = create_object();
    o->type = FIXNUM;
    o->data.fixnum.value = value;
    return o;
}














#endif