#include "utils.h"
#include "types.h"

/*

Starting point list:
    https://people.eecs.berkeley.edu/~bh/ssch27/appendix-funlist.html
Desired primitives:
    Number operations like +,-,/,*, etc...
    car,cdr,cons, null? etc....

 */



//list operations
object* primitive_list(object* list){
    //(list 1 2 3) -> (cons 1 (cons 2 (cons 3 '())))
    //Since parameters are passed as a list, just return it
    return list;
}

object* primitive_car(object* args){
    //args = (car a), where 'a' is a pair (b . '())
    arity_check(args, 1);
    return car(car(args));
}

object* primitive_cdr(object* args){
    arity_check(args, 1);
    return cdr(car(args));
}

object* primitive_cons(object* args){
    //(cons a b) => a = car(args), b = cadr(args)
    arity_check(args, 2);
    return cons(car(args), car(cdr(args)));
}

//Math operations
object* primitive_add(object* list){
    int r = 0;
    while (!is_empty_list(list)){
        object* a = car(list);
        //type check
        if (is_fixnum(a)){
            r += a->data.fixnum.value;
        }
        else{         
            return error_object("Expected a number when adding");
        }
        list = cdr(list);
    }
    return make_fixnum(r);
}

object* primitive_sub(object* list){
    int r = 0;
    while (!is_empty_list(list)){
        object* a = car(list);
        //type check
        if (is_fixnum(a)){
            r -= a->data.fixnum.value;
        }
        else{         
            return error_object("Expected a number when adding");
        }
        list = cdr(list);
    }
    return make_fixnum(r);
}

object* primitive_mul(object* list){
    int r = 0;
    while (!is_empty_list(list)){
        object* a = car(list);
        //type check
        if (is_fixnum(a)){
            r *= a->data.fixnum.value;
        }
        else{         
            return error_object("Expected a number when adding");
        }
        list = cdr(list);
    }
    return make_fixnum(r);
}

object* primitive_div(object* list){
    int r = 0;
    while (!is_empty_list(list)){
        object* a = car(list);
        //type check
        if (is_fixnum(a)){
            if (a->data.fixnum.value == 0){
                return error_object("Integer division by 0 is illegal");
            }
            r /= a->data.fixnum.value;
        }
        else{         
            return error_object("Expected a number when adding");
        }
        list = cdr(list);
    }
    return make_fixnum(r);
}

object* primitive_modulo(object* arg){
    //No arity check, expect 2 arguments: a % b
    if (!is_pair(arg)){
        return error_object("Expected a pair when performing modulo");;
    }
    else if(!is_pair(cdr(arg))){      
        return error_object("Expected a pair when performing modulo");;
    }

    object* a = car(arg);
    object* b = car(cdr(arg));

    if (b->data.fixnum.value == 0){
        return error_object("Cannot perform modulo by 0");
    }
    int r = a->data.fixnum.value % b->data.fixnum.value;
    return make_fixnum(r);
}




//Boolean operations
object* primitive_not(object* arg){
    if (is_true(arg)){
        return ofalse;
    }
    else{
        return otrue;
    }
}

object* primitive_eq(object* arg){
    //Equality if 2 objects share the same address
    //Exceptions: Numbers, booleans and characters are checked by values

    //expecting (list a b) for testing equality a == b
    if (!is_pair(arg)){
        return error_object("Expected a pair when testing equality");;
    }
    else if(!is_pair(cdr(arg))){      
        return error_object("Expected a pair when testing equality");;
    }

    object* a = car(arg);
    object* b = car(cdr(arg));

    if (a->type != b->type){
        return error_object("Equality should be between the same types");
    }

    switch(a->type){
        case FIXNUM:
            if (a->data.fixnum.value == b->data.fixnum.value){
                return otrue;
            }
            else{
                return ofalse;
            }
        case CHARACTER:
            if (a->data.character.value == b->data.character.value){
                return otrue;
            }
            else{
                return ofalse;
            }
        case BOOLEAN:
            //Bools normally only return the otrue or ofalse object so a pointer equality could work
            if (a->data.boolean.value == b->data.boolean.value){
                return otrue;
            }
            else{
                return ofalse;
            }
        default:
            return a == b ? otrue : ofalse;
    }
}

object* primitive_lt(object* arg){
    //(< a b), between 2 numbers

    //expecting (list a b) for testing equality a == b
    if (!is_pair(arg)){
        return error_object("Expected a pair when testing less than");;
    }
    else if(!is_pair(cdr(arg))){      
        return error_object("Expected a pair when testing less than");;
    }

    object* a = car(arg);
    object* b = car(cdr(arg));

    if (a->type != b->type){
        return error_object("Less than should be between the same types");
    }

    switch(a->type){
        case FIXNUM:
            return (a->data.fixnum.value < b->data.fixnum.value) ? otrue : ofalse;
        default:
            return error_object("'<' supported only for numbers");
    }
}

object* primitive_gt(object* arg){
    //(> a b), between 2 numbers

    //expecting (list a b) for testing equality a == b
    if (!is_pair(arg)){
        return error_object("Expected a pair when testing greater than");;
    }
    else if(!is_pair(cdr(arg))){      
        return error_object("Expected a pair when testing greater than");;
    }

    object* a = car(arg);
    object* b = car(cdr(arg));

    if (a->type != b->type){
        return error_object("Greater than should be between the same types");
    }

    switch(a->type){
        case FIXNUM:
            return (a->data.fixnum.value > b->data.fixnum.value) ? otrue : ofalse;
        default:
            return error_object("'>' supported only for numbers");
    }
}

object* primitive_le(object* arg){
    //(<= a b), between 2 numbers

    //expecting (list a b) for testing equality a == b
    if (!is_pair(arg)){
        return error_object("Expected a pair when testing less-equal than");;
    }
    else if(!is_pair(cdr(arg))){      
        return error_object("Expected a pair when testing less-equal than");;
    }

    object* a = car(arg);
    object* b = car(cdr(arg));

    if (a->type != b->type){
        return error_object("Less-equal than should be between the same types");
    }

    switch(a->type){
        case FIXNUM:
            return (a->data.fixnum.value <= b->data.fixnum.value) ? otrue : ofalse;
        default:
            return error_object("'<=' supported only for numbers");
    }
}

object* primitive_ge(object* arg){
    //(>= a b), between 2 numbers

    //expecting (list a b) for testing equality a == b
    if (!is_pair(arg)){
        return error_object("Expected a pair when testing greater-equal than");;
    }
    else if(!is_pair(cdr(arg))){      
        return error_object("Expected a pair when testing greater-equal than");;
    }

    object* a = car(arg);
    object* b = car(cdr(arg));

    if (a->type != b->type){
        return error_object("Greater-equal than should be between the same types");
    }

    switch(a->type){
        case FIXNUM:
            return (a->data.fixnum.value >= b->data.fixnum.value) ? otrue : ofalse;
        default:
            return error_object("'>=' supported only for numbers");
    }
}