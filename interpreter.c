#include "reader.h"

#include <stdio.h>

int main(int argc, char const *argv[])
{
    
    object *o = read(stdin);
    printf("recived object o with type %s\n", enum_strings[o->type]);





    return 0;
}



