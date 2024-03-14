#include "types.h"
#include "utils.h"
#include "string_operations.h"




int write_sexpr(FILE *out, object* sexpr);



int write_procedure(FILE *out, object* procedure){
    int return_value = fprintf(out, "#procedure");
    if (procedure->type == COMPOUND){
        return_value = write_sexpr(out, procedure->data.compound.params);
    }
    return return_value;
}




int write_pair(FILE *out, object* pair){
    //If cdr is not a pair or the empty list then print the dot
    //Else, print the car and recursively call it, unless it's the empty list in which case
    //nothing is printed
    int return_value = 0;
    if (is_pair(cdr(pair))){
        write_sexpr(out, car(pair));
        fprintf(out, " ");
        return_value = write_pair(out, cdr(pair));
    }
    else if (is_empty_list(cdr(pair))){
        return_value = write_sexpr(out, car(pair));
    }
    else{ //a pair
        write_sexpr(out, car(pair));
        fprintf(out, " . ");
        return_value = write_sexpr(out, cdr(pair));
    }
   
    return return_value;
}

int write_sexpr(FILE *out, object* sexpr){
    //Those s-expr that self-evaluate and the empty list
    int return_value = 0;
    switch(sexpr->type){
        case EMPTY_LIST:
            return_value = fprintf(out, "'() ");
            break;
        case FIXNUM:
            return_value = fprintf(out, "%d", sexpr->data.fixnum.value);
            break;
        case SYMBOL:
            return_value = fprintf(out, "%s", sexpr->data.symbol.value);
            break;
        case BOOLEAN:
            if (sexpr->data.boolean.value){
                return_value = fprintf(out, "#t ");
            }
            else{
                return_value = fprintf(out, "#f ");
            }
            break;
        case CHARACTER:
            return_value = fprintf(out, "%c ", sexpr->data.character.value);
            break;
        case ERROR:
        case STRING:
            return_value = fprintf(out, "%s ", sexpr->data.string.value);
            break;
        case PAIR:
            fprintf(out, "(");
            return_value = write_pair(out, sexpr);
            fprintf(out, ")");
            break;
        case PRIMITIVE:
        case COMPOUND:
            return_value = write_procedure(out, sexpr);
            break;

        default:
            error("Should not reach the default case in write_sexpr");
    }    

    
    return return_value;
}


void write(FILE *out, object* o){
    int return_value = write_sexpr(out, o);
    if (return_value < 0){
        error("Error in writing to stream");
    }
    fprintf(out, "\n");
}

void write_stdin(object* arg){
    write(stdout, arg);
}
