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
    return malloc(size);
}

object *create_object(){
    object *obj;

    obj = allocate(sizeof obj);
    if (obj == NULL){
        error("Ran out of memory");
        exit(1);
    }


    return obj;
}

int is_empty_list(object* list){
    return list == empty_list;
}

int is_pair(object* pair){
    return pair->type == PAIR;
}


object* cons(object *car, object *cdr){
    object *cons = create_object();
    cons->type = PAIR;
    
    cons->data.pair.car = car;
    cons->data.pair.cdr = cdr;

    return cons;
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















#endif