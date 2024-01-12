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











#endif