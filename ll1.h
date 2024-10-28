#ifndef ll1
#define ll1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"
#include "gen_utils.h"
#include "parsing_utils.h"


struct parsing_table generate_ll1_parsing_table(grammar G){
    struct parsing_table p;
    p.type=LL1;
    p.num_items=0;
    p.num_term=G.num_term+1;
    p.num_non_term=G.num_non_term;
    p.table=(char***)malloc(p.num_items*sizeof(char**));
    int len_table_col=0;

    fill_parsing_table(&p);

    // print_parsing_table(G,p);

    for(int i=0;i<G.num_rules;i++){
        printf("\n");
        int len_first_set=0;
        char* first_set=(char*)malloc(100*sizeof(char));
        char* symbol_production=G.production_rules[i][1];
        for(int j=0;symbol_production[j]!='\0';j++){
            int len_visited_arr=0;
            char* visited_arr=(char*)malloc(100*sizeof(char));
            first(G,visited_arr,&len_visited_arr,first_set,&len_first_set,symbol_production[j],'\0',symbol_production[j]);
            free(visited_arr);
            if(is_in_arr('#',first_set,len_first_set)){
                if(symbol_production[j+1]!='\0'){
                    remove_from_arr(first_set,'#',&len_first_set);
                }
            }
            else{
                break;
            }
        }
        
        for(int j=0;j<len_first_set;j++){
            int row_pos=is_in_arr(G.production_rules[i][0][0],G.non_term,G.num_non_term)-1;
            int col_pos=is_in_arr(first_set[j],G.term,G.num_term)-1;
            int production_pos=production_position(G.production_rules[i],G);
            if(p.table[row_pos][col_pos][0]==-1){
                if(first_set[j]!='#'){
                    p.table[row_pos][col_pos][0]=production_pos;
                }
                else{
                    int len_temp_visited_arr=0;
                    char* temp_visited_arr=(char*)malloc(100*sizeof(char));
                    int len_follow_set=0;
                    char* follow_set=(char*)malloc(100*sizeof(char));
                    follow(G,temp_visited_arr,&len_temp_visited_arr,follow_set,&len_follow_set,G.production_rules[i][0][0]);
                    for (int k=0;k<len_follow_set;k++){
                        if(follow_set[k]=='$'){
                            p.table[row_pos][p.num_term-1][0]=production_pos;
                        }
                        else{
                            p.table[row_pos][col_pos][0]=production_pos;
                        }
                    }
                }
            }
            else{
                printf("Conflict\n");
                exit(0);
            }
        }
    }
    return p;
}

void parse(grammar G,struct parsing_table p,char* string){
    struct stack* s=(struct stack*)malloc(1*sizeof(struct stack));
    s->arr=(char*)malloc(100*sizeof(char));
    s->top=-1;
    push(s,'$');
    push(s,G.start_symbol);

    int lookahead_point=0;
    string[strlen(string)+1]='\0';
    string[strlen(string)]='$';

    printf("Stack\tTop\tString\tLA\n");

    while(1){
        for(int i=0;i<(s->top)+1;i++){
            printf("%c",s->arr[i]);
        }
        printf("\t%d\t%s\t%d",s->top,string,lookahead_point);
        
        
        break;
        // printf("\t%c%d\n",action[0],action[1]);
    }
    printf("\n");
}

#endif