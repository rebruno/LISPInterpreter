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
    object* o2;

    FILE* in = stdin;
    FILE* out = stdout;

    int reading_from_file = 0;
    if (argc > 1){
        in = fopen(argv[1], "r");
        reading_from_file = 1;
        if (in == NULL){
            printf("Cannot open the file %s\n", argv[1]);
            return -1;
        }
    }

    while (1){ 
        printf("> ");
        o = read(in);
        if (o->type == ERROR){
            if (ferror(in)){
                printf("Encountered an error in reading\n");    
                break;
            }
            //EOF but no error, just continue to terminal by using stdin
            fclose(in);
            in = stdin;
            reading_from_file = 0;
            continue;
        }
        o2 = eval(o, env);
        if (reading_from_file){
            write(out, o);
        }
        write(out, o2);
    }

    printf("Exiting\n");
    
    fclose(in); 

    return 0;
}



