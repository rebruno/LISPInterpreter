#ifndef LISPSTR
#define LISPSTR



#include "types.h"
#include "utils.h"
#include <ctype.h>
#include <string.h>

//object* string_to_number()



int isdelim(int c){
    return isspace(c) || c == ')' || c == '(' || c == '"' || c == ';' || c == EOF;
}

int isnumeric(const char c){ 
    //Can be replaced later to support non-ASCII digits?
    return isdigit(c); 
}

int stringeq(const char * s1, const char * s2){
    return strcmp(s1, s2);
}

int stringlength(const char * s){
    return strlen(s);
}

int stringncopy(char * s1, char * s2, int n){
    //n should be the size of s1 and s2 is null terminated
    int len = stringlength(s2);
    for (int i = 0; i < len && i < n; i++){
        s1[i] = s2[i];
    }
    s1[len] = 0;
    return 0;
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


enum{
    NAN = -1,
    NUM = 0,
};


int string_to_number(const char* str, Number *n){

    int i = 0;
    int j = 0;
    int e = 0;  

    int minusflag = 0;

    const char *c = str;
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
    else if (*c != 0 && !isnumeric(*c)){
        return NAN;
    }
    else { //integer
        n->data.i = (minusflag == 0) ? i : -i;
        n->numbertype = INT;
    }



    return NUM; 
}


object *create_number (const char* str){
    Number n;
    int result = string_to_number(str, &n);
    if (result == NUM){
        object *o = create_object();
        o->type = FIXNUM; //only do integers for now
        o->data.fixnum.value = n.data.i;
        return o;
    }
    else if (result == NAN){
        return 0L;
    }
    return 0L; // proper error handling to be done later
}






















#endif