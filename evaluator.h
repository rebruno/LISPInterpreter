#include "types.h"
#include "utils.h"


object* eval(object *exp, object *env);

void add_binding_to_frame(object* var, object* val, object* frame){
    set_car(frame, cons(var, car(frame)));
    set_cdr(frame, cons(val, cdr(frame)));
}

object* lookup(object* id, object* env){
    //lookup() returns the value associated with a 
    //Env is a list of frames, where a frame is (cons variables values)
    //Lookup consists of just going through entire list
    
    object *frame, *vars, *vals;

    while (!is_empty_list(env)){
        frame = car(env);
        vars = car(frame);
        vals = cdr(frame);

        while (!is_empty_list(vars)){
            if (id == car(vars)){ 
                return car(vals);
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = cdr(env);
    }

    char buf[100];
    int i = snprintf(buf, 100, "Cannot find symbol %s", id->data.symbol.value);
    if ( i >= 0 && i < 100){
        error(buf);
    } 
    else{
        error("Cannot find symbol and name is too long to print");
    }
    return empty_list;
}

int assign_symbol(object* var, object* val, object* env){
    object *frame, *vars, *vals;

    while (!is_empty_list(env)){
        frame = car(env);
        vars = car(frame);
        vals = cdr(frame);

        while (!is_empty_list(vars)){
            if (var == car(vars)){ 
                set_car(vals, val);
                return 0;            
            }
            vars = cdr(vars);
            vals = cdr(vals);
        }
        env = cdr(env);
    }

    //not found
    return -1;
}

object* make_lambda(object* params, object* body){
    error("Have not impemented lambdas");
}

object* definition_variable(object* exp){
    object* cadr = car(cdr(exp));
    if (is_symbol(cadr)){
        return cadr;
    }
    return car(cadr);
}

object* definition_value(object* exp){
    if (is_symbol(car(cdr(exp)))){
        return car(cdr(cdr(exp)));
    }
    return make_lambda(cdr(car(cdr(exp))), cdr(cdr(exp)));
}


int define_symbol(object* var, object* val, object* env){
    object *frame, *vars, *vals;
    object* binding_pair;

    frame = car(env); //we only look through the closest frame when defining
    vars = car(frame);
    vals = cdr(frame);
    
    while (!is_empty_list(vars)){
        if (var == car(vars)){ 
            warning("Redefining an existing variable");
            set_car(vals, val);
            return 0;
        }
        vars = cdr(vars);
        vals = cdr(vals);
    }


    add_binding_to_frame(var, val, frame);
    return 0;
}

object* make_frame(object* vars, object* vals){
    return cons(vars, vals);
}


object* extend_env(object* vars, object* vals, object* env){
    //Adds the vars and vals to env as frames
    
    object* original_env = env; //Return in case of vars/vals mismatch
    object* frame;

     if (is_pair(vars) && is_pair(vals)){
        if (list_length(vars) != list_length(vals)){
            error("Mismatched number of vars and vals when extending environment");
            return empty_list;
        }
    }

    
    return cons(make_frame(vars, vals), env);
}




object* create_empty_environment(){
    // Environments are expected to be made of frames. If the empty environment is just an empty list
    // then a function trying to access it will face an error when it tries to access a variable 
    // Or value as those access the car/cdr. Instead, the empty environment is 
    // a frame with the binding   
    object* env = create_object();
    env = cons(make_frame(empty_list, empty_list), empty_list);
    return env;

}
object* populate_default_environment(object* env){
    /*
    This should have all the primitive procedures that are expected, such as '+', '*', eq?, etc...
    It adds them to the environment passed in
     */
    object* o = cons(exit_symbol, empty_list);
    return extend_env(o, o, env);
}



object* is_null(object* exp){ //Maybe make this return an int instead
    return is_empty_list(car(exp)) ? otrue : ofalse;
}

object* eval_if(object* exp, object* env){
    object* body = cdr(exp);
    object* predicate = eval(car(body), env);
    if (is_true(predicate)){
        return car(cdr(body));
    }
    else{
        if (is_empty_list(cdr(cdr(body)))){
            return cdr(cdr(body));    
        }
        return ofalse;
    }
}

object* sequence_to_exp(object* seq){
    if (is_empty_list(seq)){
        return seq;
    }
    else if (is_empty_list(cdr(seq))){
        return car(seq);
    }
    else{
        return cons(begin_symbol, seq); //make-begin
    }
}

object* make_if(object* predicate, object* consequent, object* alternative){
    return (cons(if_symbol, cons(predicate, cons(consequent, cons(alternative, empty_list)))));
}

object* expand_cond(object* clauses){
    //0 idea what this does, pretty much directly converted from Lisp code in SICP to C
    if (is_empty_list(clauses)){
        return ofalse;
    }
    
    object* first = car(clauses);
    object* rest = cdr(clauses);

    if (eq(car(first), else_symbol)){
        if (is_empty_list(rest)){
            return sequence_to_exp(cdr(first));
        }
        else{
            error("else clause isn't last in cond");
            return empty_list;
        }
    }
    else{
        make_if(car(first), sequence_to_exp(cdr(first)), expand_cond(rest));
    }
}

object* make_procedure(object* params, object* body, object* env){
    //Returns a compound procedure i.e. user defined
    object* proc = create_object();

    proc->type = COMPOUND;
    proc->data.compound.params = params;
    proc->data.compound.params = body;
    proc->data.compound.params = env;


}

object* eval(object *exp, object *env){

    if (is_self_eval(exp)){
        return exp;
    }
    else if (is_symbol(exp)){
        return lookup(exp, env);
    }
    else if (is_quoted(exp)){
        return car(cdr(exp));
    }
    else if (is_assignment(exp)){
        //set_symbol(exp, env);
        assign_symbol(car(cdr(exp)), 
            eval(cdr(cdr(exp)), env), env);
    }
    else if (is_definition(exp)){
        //define_symbol(car(cdr(exp)), eval(car(cdr(cdr(exp))), env), env);
        define_symbol(definition_variable(exp), eval(definition_value(exp), env), env);
    }
    else if (is_if(exp)){
        return eval(eval_if(exp, env), env); //For tail calls later, set exp = eval_if and just go to top
    }
    else if (is_lambda(exp)){
        return make_procedure(car(cdr(exp)), cdr(cdr(exp)), env);
    }
    else if (is_begin(exp)){
        exp = cdr(exp);
        while (!is_null(cdr(exp))){
            eval(car(exp), env);
            exp = cdr(exp);
        }
        return eval(car(exp), env);
    }
    else if (is_cond(exp)){
        object* cond_clauses = cdr(exp);
        return eval(expand_cond(cond_clauses), env);
    }
    else if (is_application(exp)){ //Kind of pointless but basically it just tests if it's a pair

    }
    else{
        error("Not yet implemented or unknown expression");
    }
    return empty_list;
}