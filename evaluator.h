#include "types.h"
#include "utils.h"

#include "primitives.h"


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
    //error("Have not impemented lambdas");
    return cons(lambda_symbol, cons(params, body));
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


object* eval_if(object* exp, object* env){
    object* body = cdr(exp);
    object* predicate = eval(car(body), env);
    if (is_error(predicate)){
        return predicate;
    }
    else if (is_true(predicate)){
        return eval(car(cdr(body)), env);
    }
    else{
        if (is_empty_list(cdr(cdr(body)))){
            return eval(cdr(cdr(body)), env);    
        }
        return eval(car(cdr(cdr(body))), env);
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

object* make_primitive(object* (*function)(struct object* args)){
    object* primitive_proc = create_object();
    primitive_proc->type = PRIMITIVE;
    primitive_proc->data.primitive.function = function;
    return primitive_proc;
}

object* make_procedure(object* params, object* body, object* env){
    //Returns a compound procedure i.e. user defined
    object* proc = create_object();

    proc->type = COMPOUND;
    proc->data.compound.params = params;
    proc->data.compound.body = body;
    proc->data.compound.env = env;

    return proc;
}

object* eval_sequence(object* exp, object* env){
    //Currently, car(exp) must be the begin symbol
    //Should be modified so it is just the first pair to execute
    exp = cdr(exp); 
    object* val;
    while (!is_empty_list(exp)){
        val = eval(car(exp), env);
        exp = cdr(exp);
    }
    return val;
}

object* apply_primitive(object* procedure, object* arguments){
    //error("Have not implemented primitives yet");
    #define primitive_function(p) p->data.primitive.function
    object* result = (primitive_function(procedure))(arguments);
    return result;
}

object* apply(object* procedure, object* arguments){
    if (procedure->type == PRIMITIVE){
        return apply_primitive(procedure, arguments);
    }
    else if (procedure->type == COMPOUND){
        #define procedure_parameters(p) p->data.compound.params
        #define procedure_body(p) p->data.compound.body
        #define procedure_env(p) p->data.compound.env

        return eval_sequence(cons(begin_symbol, procedure_body(procedure)), 
                            extend_env( procedure_parameters(procedure), 
                                        arguments, procedure_env(procedure)));
    }
    else{
        return error_object("Unknown procedure type when trying to apply()");
    }
}

object* evaluate_args(object* expressions, object* env){
    //list-of-values in SICP
    if (is_empty_list(expressions)){
        return empty_list;
    }
    else{
        return cons(eval(car(expressions), env),
                    evaluate_args(cdr(expressions), env));
    }
}

object* populate_default_environment(object* env){
    /*
    This should have all the primitive procedures that are expected, such as '+', '*', eq?, etc...
    It adds them to the environment passed in
     */

    #define primitive_procedure(name, primitive_name) define_symbol(make_symbol(name), make_primitive(primitive_name), env);

    primitive_procedure("+", primitive_add);
    primitive_procedure("-", primitive_sub);
    primitive_procedure("*", primitive_mul);
    primitive_procedure("/", primitive_div);
    primitive_procedure("%", primitive_modulo);

    //list operations
    primitive_procedure("list", primitive_list);
     primitive_procedure("length", primitive_list_length);
    primitive_procedure("car", primitive_car);
    primitive_procedure("cdr", primitive_cdr);
    primitive_procedure("cons", primitive_cons);

    //booleans
    primitive_procedure("not", primitive_not);
    primitive_procedure("eq?", primitive_eq);
    primitive_procedure("null?", primitive_null);
    primitive_procedure(">", primitive_gt);
    primitive_procedure("<", primitive_lt);
    primitive_procedure("<=", primitive_le);
    primitive_procedure(">=", primitive_ge);

    //I/O
    primitive_procedure("read", primitive_read);
    primitive_procedure("write", primitive_write);


    //Found in utils.h because it uses the standard library
    primitive_procedure("exit", primitive_exit);

    return env;
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
        int result = assign_symbol(car(cdr(exp)), 
            eval(cdr(cdr(exp)), env), env);
        if (result < 0){
            error("Did not find variable to assign to");
        }
    }
    else if (is_definition(exp)){
        define_symbol(definition_variable(exp), eval(definition_value(exp), env), env);
    }
    else if (is_if(exp)){
        return eval_if(exp, env); //For tail calls later, set exp = eval_if and just go to top
    }
    else if (is_lambda(exp)){
        return make_procedure(car(cdr(exp)), cdr(cdr(exp)), env);
    }
    else if (is_begin(exp)){
        return eval_sequence(exp, env);
    }
    else if (is_cond(exp)){
        object* cond_clauses = cdr(exp);
        return eval(expand_cond(cond_clauses), env);
    }
    else if (is_application(exp)){ //Kind of pointless but basically it just tests if it's a pair
        #define operator(e) car(e)
        #define operands(e) cdr(e)
        return apply(eval(operator(exp), env), evaluate_args(operands(exp),env));
    }
    else{
        error("Not yet implemented or unknown expression");
    }
    return empty_list;
}