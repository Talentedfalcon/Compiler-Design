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

#endif