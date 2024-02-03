#include "reader.h"
#include "evaluator.h"
#include "writer.h"

#include <stdio.h>

int main(int argc, char const *argv[])
{
    printf("Init symbols\n");
    init_symbols();

    printf("Creating empty env\n");
    object* env = create_empty_environment();

    printf("Populating\n");
    env = populate_default_environment(env);

    object* o = empty_list;

    while (!is_exit(o)){ //Exit should be replaced by a proper procedure
        printf("> ");
        o = read(stdin);

        //For debug
        printf("Received: \n");
        write(stdout, o);

        o = eval(o, env);

        printf("Evaluated: \n");
        write(stdout, o);
    }

    printf("Exiting\n");
    return 0;
}



