#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clr1.h"

int main(){
    grammar G=input_grammar();
    grammar aug_G=augment_grammar(G);
    print_grammar(aug_G);

    lr1_item_node* item_list=(lr1_item_node*)malloc(100*sizeof(lr1_item_node));
    int len_item_list=0;

    char* lookaheads=(char*)malloc(100*sizeof(char));
    lookaheads[0]='$';
    item_list[len_item_list++].i=init_item(aug_G.production_rules[aug_G.num_rules-1],0,lookaheads);

    lr1_closure(aug_G,&item_list[0].i);
    int j=0;
    while(j<len_item_list){
        lr1_GOTO(aug_G,item_list,&len_item_list,j);
        j++;
    }

    printf("\n");
    print_item_list(item_list,len_item_list);

    struct parsing_table p=generate_clr1_parsing_table(G,item_list,len_item_list);
    printf("\nCLR Table:\n");
    print_parsing_table(G,p);

    printf("\n");
    char* string=(char*)malloc(100*sizeof(char));
    printf("Input String: ");
    scanf("%s",string);

    printf("\n");
    parse(G,p,string);

    return 0;
}