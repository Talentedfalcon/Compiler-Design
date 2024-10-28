#ifndef clr1
#define clr1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"
#include "gen_utils.h"
#include "lr1_utils.h"
#include "parsing_utils.h"

struct parsing_table generate_clr1_parsing_table(grammar G,lr1_item_node* item_list,int len_item_list){
    struct parsing_table p;
    p.type=LR1;
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

void parse(grammar G,struct parsing_table p,char* string){
    struct stack* s=(struct stack*)malloc(1*sizeof(struct stack));
    s->arr=(char*)malloc(100*sizeof(char));
    s->top=-1;
    push(s,'$');
    push(s,0);

    int lookahead_point=0;
    string[strlen(string)+1]='\0';
    string[strlen(string)]='$';

    printf("Stack\tTop\tString\tLA\tNext Action\n");
    while(1){
        int pos;
        char* action;
        if(string[lookahead_point]=='$'){
            action=p.table[top(s)][p.num_term-1];
        }
        else if((pos=is_in_arr(string[lookahead_point],G.term,G.num_term))){
            action=p.table[top(s)][pos-1];
        }
        else if((pos=is_in_arr(string[lookahead_point],G.non_term,G.num_non_term))){
            action=p.table[top(s)][pos-1];
        }
        else{
            printf("Rejected\n");
            return;
        }

        display_stack(s);
        printf("\t%s\t%d",string,lookahead_point);
        printf("\t%c%d\n",action[0],action[1]);
        
        if(action[0]=='E'){
            printf("Rejected\n");
            return;
        }
        else if(action[0]=='A'){
            printf("ACCEPTED\n");
            return;
        }
        else if(action[0]=='S'){
            push(s,string[lookahead_point++]);
            push(s,action[1]);
        }
        else if(action[0]=='R'){
            char** production=G.production_rules[action[1]-1];
            int pop_count=(strlen(production[1]))*2;
            while((pop_count--)>0){
                pop(s);
            }
            display_stack(s);
            printf("\t%s\t%d",string,lookahead_point);
            int prev_top=top(s);
            push(s,production[0][0]);
            pos=is_in_arr(top(s),G.non_term,G.num_non_term);
            if(pos){
                char* goto_action=p.table[prev_top][p.num_term+pos-1];
                printf("\t%c%d\n",goto_action[0],goto_action[1]);
                if(goto_action[0]=='G'){
                    push(s,goto_action[1]);
                }
                else{
                    printf("Unknow Action\n");
                    return;
                }
            }
        }
        else{
            printf("Unknow Action\n");
            return;
        }
    }
}

#endif