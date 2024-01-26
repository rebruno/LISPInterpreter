#include "types.h"
#include "utils.h"
#include "string_operations.h"




#include <stdio.h>

#include <stdlib.h>


object empty_list_obj = { .type = EMPTY_LIST, .data = 0};
object* empty_list = &(empty_list_obj);

object otrue_obj = { .type = BOOLEAN, .data.boolean = true};
object* otrue = &(otrue_obj);


object ofalse_obj = { .type = BOOLEAN, .data.boolean = false};
object* ofalse = &(ofalse_obj);

object* quote_symbol;

object* symbols = &(empty_list_obj);


int add_symbol(object* s){
    if (s->type != SYMBOL){
        return -1;
    }
    symbols = cons(s, symbols);
    return 0;
}

object* lookup_symbol(const char* v){
    object* current_symbols = symbols;
    while (!is_empty_list(current_symbols)){
        object * s = car(current_symbols);
        if (stringeq(s->data.symbol.value, v) == 0){
            return s;
        }
        current_symbols = cdr(current_symbols);
    }
    return empty_list;
}

#define BUFFER_SIZE 1000

//wav.c from elsewhere has a better example of doing this
#define ERRORHANDLE(c) \
    if (c == EOF){\
        goto handleEOF;\
    } 



//Error handling is left to where these are used
int get_next_char(FILE *in){ //peek
    int c = fgetc(in);
    ungetc(c, in);
    return c; 
}

int consume_next_char(FILE *in){ 
    return fgetc(in);
}




void eat_whitespace(FILE *in){
    int c;
    while ((c = get_next_char(in)) != EOF && isspace(c)){
        consume_next_char(in);
    }

}





object *read(FILE *in);

object *readpair(FILE *in){
    //Starts on first character after the opening '('
    eat_whitespace(in); //maybe this should also eat opening brackets?

    object *caro;
    object *cdro;

    int c = get_next_char(in);

    if (c == '('){
        consume_next_char(in);
        c = get_next_char(in);
    }

    if (c == EOF){
        goto handleEOF;
    }


    if (c == ')'){
        return empty_list;
    }

    caro = read(in);
    eat_whitespace(in);

    c = get_next_char(in);
    if (c == '.'){
        consume_next_char(in);
        ERRORHANDLE(c)

        cdro = read(in);
        eat_whitespace(in);

        if (get_next_char(in) != ')'){
            error("Closing paren missing in readpair");
            return empty_list;
        }
        //return cons(caro, cdro);
    }
    else{
        cdro = readpair(in);
        eat_whitespace(in);

        if (get_next_char(in) != ')'){
            error("Closing paren missing");
            return empty_list;
        }
        //return cons(caro, cdro);
    }


handleEOF:
    if (ferror(in) != 0){
        error("Error with input stream");
        //For now, return empty list. 
        return empty_list;
    }

    return cons(caro, cdro);
}




object *readstring(FILE *in){
    //assumes that the first quotation mark is still in the stream
    //must be skipped
    consume_next_char(in);


    char *buf = allocate(BUFFER_SIZE * sizeof *buf);  //not sure if using int or char is better

    object *o = create_object();
    int i = 0;

    int c = get_next_char(in);

    //while (i < BUFFER_SIZE && !isdelim(c)){ //maybe && c != '"' instead of any delim
    while (i < BUFFER_SIZE && c != '"'){ 
        c = consume_next_char(in);
        buf[i++] = c;
    }
    //if (i == BUFFER_SIZE || !isdelim(get_next_char(in))){
    if (i == BUFFER_SIZE || !isdelim(get_next_char(in))){
        error("Exceeded buffersize");
        return empty_list;
    }

    //last char was delim
    buf[i-1] = '\0';

    o->type = STRING;
    o->data.string.value = buf;

    return o;
}

object *make_char(const char *str){
    // Assumes input of form #\x where x is a character. 
    const char *c = str;
    
    if ((str[0] != '#' && str[1] != '\\') || str[2] == 0){
        return empty_list;
    }   

    object *o = create_object();
    o->type = CHARACTER;

    o->data.character.value = str[2]; //When supporting forms like #\u03BB, will need to change this

    return o;
}

object *make_symbol(const char *buffer){
    /*
    From "LISP in Small Pieces" ch 1.5
    Creating symbols should involve looking them up, to see if they exist
    If they do, then just return that symbol. This allows using pointer equality
     */
    
    object * newsymbol;

    newsymbol = lookup_symbol(buffer);
    if (!is_empty_list(newsymbol)){
        return newsymbol;
    }


    newsymbol = create_object();
    newsymbol->type = SYMBOL;
    newsymbol->data.symbol.value = allocate(stringlength(buffer) + 1);
    add_symbol(newsymbol);

    return newsymbol;
}

object *readatom(FILE *in){
    //Reads symbols, fixnums, booleans, characters and strings
    
    int c = get_next_char(in);

    if (c == '"'){
        return readstring(in);
    }
    

    //int buf[BUFFER_SIZE] = {0};
    char buf[BUFFER_SIZE] = {0};
    int i = 0;

    while (i < BUFFER_SIZE && !isdelim((c = get_next_char(in)))){
        c = consume_next_char(in);
        buf[i++] = c;
    }

    if (i == BUFFER_SIZE || !isdelim(c)){ //might be wrong
        error("Exceeded buffersize");
        return empty_list;
    }

    //Last char was a delimiter char
    buf[i] = '\0';

    if (buf[0] == '#' && i > 0){
        if (buf[1] == 't'){
            return otrue;
        }
        else if (c == 'f'){
            return ofalse;
        }
        return make_char(buf);
    }

    if (isnumeric(buf[0]) || (i > 0 && isnumeric(buf[1]) && buf[0] == '-')){
        object *o = create_number(buf);
        //If o is not the null pointer then it is a number. If it is, that means we had something like 123abc which can be a symbol in most LISPS.
        if (o != 0){ 
            return create_number(buf);
        }

    }

    object *newsymbol = make_symbol(buf);
    if (newsymbol != 0L){
        return newsymbol;
    }

    return empty_list;
}

object *read(FILE *in){
    eat_whitespace(in);
    object* o;

    int c = get_next_char(in);

    if (c == '('){
        o = readpair(in);
        if (get_next_char(in) != ')'){
            error("Closing paren missing");
        }
        consume_next_char(in);
        return o;
    }

    else if (c == '\''){
        consume_next_char(in);
        o = read(in);
        return cons(quote_symbol, cons(o, empty_list));
    }

    return readatom(in);

}
