#include "utils.h"
#include "types.h"

object* primitive_add(object* list){
    int r = 0;
    while (!is_empty_list(list)){
        object* a = car(list);
        //type check
        if (is_fixnum(a)){
            r += a->data.fixnum.value;
        }
        else{
            error("Expected a number when adding");
            return empty_list;
        }
        list = cdr(list);
    }
    return make_fixnum(r);
}