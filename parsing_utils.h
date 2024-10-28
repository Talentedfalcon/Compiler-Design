#ifndef parsing_table_utils
#define parsing_table_utils

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grammar.h"

struct parsing_table{
    int num_items;
    int num_term;
    int num_non_term;
    char*** table;
};

void fill_parsing_table(struct parsing_table* p){
    int num_table_col=0;
    for(int i=0;i<p->num_items;i++){
        char** table_row=(char**)malloc((p->num_term+p->num_non_term)*sizeof(char*));
        int num_table_row=0;
        for(int j=0;j<p->num_term+p->num_non_term;j++){
            char* table_cell=(char*)malloc(2*sizeof(char));
            table_cell[0]='E';
            table_cell[1]=-1;
            table_row[num_table_row++]=table_cell;
        }
        p->table[num_table_col++]=table_row;
    }
}

void print_parsing_table(grammar G,struct parsing_table p){
    printf("\t");
    for(int i=0;i<p.num_term-1;i++){
        printf("%c\t",G.term[i]);
    }
    printf("$\t");
    for(int i=0;i<p.num_non_term;i++){
        printf("%c\t",G.non_term[i]);
    }
    printf("\n");

    for(int i=0;i<p.num_items;i++){
        printf("I%d\t",i);
        for(int j=0;j<p.num_term+p.num_non_term;j++){
            if(p.table[i][j][0]=='E'){
                printf("\t");
            }
            else if(p.table[i][j][0]=='A'){
                printf("Acc\t");
            }
            else{
                printf("%c%d\t",p.table[i][j][0],p.table[i][j][1]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void conflict_type(char action1,char action2){
    if(action1=='R' && action2=='R'){
        printf("RR Conflict\n");
    }
    else if((action1=='R' && action2=='S') || (action1=='S' && action2=='R')){
        printf("SR Conflict\n");
    }
    else{
        printf("Conflict");
    }
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