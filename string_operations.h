#ifndef LISPSTR
#define LISPSTR



#include "types.h"
#include "utils.h"
#include <ctype.h>

//object* string_to_number()



int isdelim(int c){
    return isspace(c) || c == ')' || c == '(' || c == '"' || c == ';' || c == EOF;
}

int isnumeric(int c){ 
    //Can be replaced later to support non-ASCII digits?
    return isdigit(c); 
}



#define INT 0
#define FLOAT 1

typedef struct Number{
    int numbertype;
    union {
        int i;
        float f;
    } data;
} Number;




int string_to_number(const int* str, Number *n){

    int i = 0;
    int j = 0;
    int e = 0;  

    int minusflag = 0;

    const int *c = str;
    if (*c == '-'){
        minusflag = 1;
    }
    
    while (*c != 0 && isnumeric(*c)){
        i = i * 10 + (*c - '0');
        c++;
    }


    if (*c == '.'){
        while (*c != 0 && isnumeric(*c)){
            i = i * 10 + (*c - '0');
            e--;
            c++;
        }
        double d = (double) i;
        while (e < 0){
            e++;
            d *= 0.1;
        }
        n->data.f = (minusflag == 0) ? d : -d;;
        n->numbertype = FLOAT;
    }
    else { //integer
        n->data.i = (minusflag == 0) ? i : -i;
        n->numbertype = INT;
    }



    return 0; 
}


object *create_number (const int* str){
    Number n;
    if (string_to_number(str, &n) == 0){
        object *o = create_object();
        o->type = FIXNUM; //only do integers for now
        o->data.fixnum.value = n.data.i;
        return o;
    }
    return 0L; // proper error handling to be done later
}






















#endif