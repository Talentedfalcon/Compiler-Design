#ifndef clr1
#define clr1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"
#include "gen_utils.h"
#include "lr1_utils.h"
#include "parsing_table_utils.h"

struct parsing_table generate_clr1_parsing_table(grammar G,lr1_item_node* item_list,int len_item_list){
    struct parsing_table p;
    p.num_items=len_item_list;
    p.num_term=G.num_term+1;
    p.num_non_term=G.num_non_term;
    p.table=(char***)malloc(p.num_items*sizeof(char**));
    int len_table_col=0;

    fill_parsing_table(&p);

    // print_parsing_table(G,p);
    
    for(int i=0;i<p.num_items;i++){
        if(item_list[i].num_directions!=item_list[i].i.num_rules){
            for(int j=0;j<item_list[i].i.num_rules;j++){
                char** production=item_list[i].i.production_rules[j];
                int dot_pos=item_list[i].i.dot_pos[j];
                if(strlen(production[1])==dot_pos+1){
                    if(production[0][0]=='@'){
                        char* temp="A";
                        p.table[i][p.num_term-1]=temp;
                    }
                    else{
                        char* temp=(char*)malloc(2*sizeof(char));
                        temp[0]='R';
                        for(int k=0;k<G.num_rules;k++){
                            char** temp_production=remove_dot(production);
                            if(compare_productions(G.production_rules[k],temp_production)){
                                temp[1]=k+1;
                                char* lookaheads=item_list[i].i.lookaheads[j];
                                for(int l=0;l<strlen(lookaheads);l++){
                                    int pos=is_in_arr(lookaheads[l],G.term,G.num_term);
                                    if(!pos){
                                        if(p.table[i][p.num_term-1][0]=='E'){
                                            p.table[i][p.num_term-1]=temp;
                                        }
                                        else{
                                            conflict_type(p.table[i][p.num_term-1][0],temp[0]);
                                            exit(0);
                                        }
                                    }
                                    else{
                                        if(p.table[i][pos-1][0]=='E'){
                                            p.table[i][pos-1]=temp;
                                        }
                                        else{
                                            conflict_type(p.table[i][pos-1][0],temp[0]);
                                            exit(0);
                                        }
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }
        for(int j=0;j<item_list[i].num_directions;j++){
            char* direction=item_list[i].direction[j];
            int pos;
            if((pos=is_in_arr(direction[0],G.term,G.num_term))){
                char* temp=(char*)malloc(2*sizeof(char));
                temp[0]='S';
                temp[1]=direction[1];
                if(p.table[i][pos-1][0]=='E'){
                    p.table[i][pos-1]=temp;
                }
                else{
                    conflict_type(p.table[i][pos-1][0],temp[0]);
                    exit(0);
                }
            }
            else if((pos=is_in_arr(direction[0],G.non_term,G.num_non_term))){
                char* temp=(char*)malloc(2*sizeof(char));
                temp[0]='G';
                temp[1]=direction[1];
                p.table[i][p.num_term+pos-1]=temp;
            }
        }
    }
    return p;
}



#endif