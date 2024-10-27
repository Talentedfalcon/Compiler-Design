#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gen_utils.h"

typedef struct{
    int num_term;
    char* term;
    int num_non_term;
    char* non_term;
    int num_rules;
    char*** production_rules;
    char start_symbol;
}grammar;

grammar input_grammar(){
    grammar G;
    G.num_term=0;
    G.num_non_term=0;
    G.num_rules=0;

    char*** production_rules=(char***)malloc(100*sizeof(char**));
    printf("Production Rules: (type @ to complete input)\n");
    while(1){
        printf("\nProduction %d:\n",G.num_rules+1);
        char** production=(char**)malloc(2*sizeof(char*));
        char* rhs=(char*)malloc(100*sizeof(char));
        printf("LHS: ");
        scanf("%s",rhs);
        production[0]=rhs;
        
        if(rhs[0]=='@'){
            break;
        }
        
        char* lhs=(char*)malloc(100*sizeof(char));
        printf("RHS: ");
        scanf("%s",lhs);
        
        if(lhs[0]=='@'){
            break;
        }
        
        int has_2_productions=0;
        int j=0;
        char* temp=(char*)malloc(100*sizeof(char));
        for(int i=0;i<strlen(lhs);i++){
            if(has_2_productions){
                temp[j]=lhs[i];
                j++;
                continue;
            }
            if(lhs[i]!='|'){
                temp[i]=lhs[i];
            }
            else{
                temp[i]='\0';
                production[1]=temp;
                has_2_productions=1;
                production_rules[G.num_rules++]=production;
                temp=(char*)malloc(100*sizeof(char));
                production=(char**)malloc(2*sizeof(char*));
                production[0]=rhs;
                j=0;
            }
        }
        production[1]=temp;
        production_rules[G.num_rules++]=production;
    }
    G.production_rules=production_rules;

    int num_non_term=0;
    char* non_term=(char*)malloc(100*sizeof(char));
    for(int i=0;i<G.num_rules;i++){
        int val=G.production_rules[i][0][0];
        if(!is_in_arr(val,non_term,num_non_term)){
            non_term[num_non_term++]=val;
        }
    }
    G.num_non_term=num_non_term;
    G.non_term=non_term;
    
    int num_term=0;
    char* term=(char*)malloc(100*sizeof(char));
    for(int i=0;i<G.num_rules;i++){
        for(int k=0;G.production_rules[i][1][k]!='\0';k++){
            char val=G.production_rules[i][1][k];
            if(!(is_in_arr(val,G.non_term,G.num_non_term) || is_in_arr(val,term,num_term))){
                term[num_term++]=val;
            }
        }
    }
    G.num_term=num_term;
    G.term=term;
    
    G.start_symbol=G.production_rules[0][0][0];
    return G;
}

void print_grammar(grammar G){
    printf("\nTerminals: ");
    for(int i=0;i<G.num_term;i++){
        printf("%c ",G.term[i]);
    }
    printf("\n\nNon Terminals: ");
    for(int i=0;i<G.num_non_term;i++){
        printf("%c ",G.non_term[i]);
    }
    printf("\n\nGrammar:\n");
    for(int i=0;i<G.num_rules;i++){
        printf("\t%s -> %s\n",G.production_rules[i][0],G.production_rules[i][1]);        
    }
}