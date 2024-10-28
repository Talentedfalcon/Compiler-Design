#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ll1.h"

int main(){
    grammar G=input_grammar();
    print_grammar(G);

    struct parsing_table p=generate_ll1_parsing_table(G);
    printf("LL Table:\n");
    print_parsing_table(G,p);

    return 0;
}