#ifndef Grammar
#define Grammar

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

int has_non_terminal=0;
int go_next=0;
int is_last_symbol=0;
int epsilon_found=0;

//Function to input and return a grammar
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

//Function to print out the grammar and some of it's details
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

//Return all the RHS productions made by 'symbol' for a given grammar
char** generate_production(grammar G,int* len_symbol_production,char symbol){
    char** result=(char**)malloc(100*sizeof(char*));
    for(int i=0;i<G.num_non_term;i++){
        if(G.non_term[i]==symbol){
            for(int i=0;i<G.num_rules;i++){
                if(symbol==G.production_rules[i][0][0]){
                    result[(*len_symbol_production)++]=G.production_rules[i][1];
                }
            }
        }
    }
    char* tempsymbol=(char*)malloc(1*sizeof(char));
    tempsymbol="\0";
    result[(*len_symbol_production)]=tempsymbol;
    return result;
}

//Compare 2 productions and return True if they are equal
int compare_productions(char** prod1,char** prod2){
    if(strcmp(prod1[0],prod2[0])){
        return 0;
    }
    else if(strcmp(prod1[1],prod2[1])){
        return 0;
    }
    return 1;
}

//Returns the position of a given production
int production_position(char** key_prod,grammar G){
    int pos=0;
    for(int i=0;i<G.num_rules;i++){
        if(compare_productions(G.production_rules[i],key_prod)){
            pos=i+1;
            break;
        }
    }
    return pos;
}

//Give the first of the 'symbol' for a given grammar
void first(grammar G,char* visited_arr,int* len_visited_arr,char* result_arr,int* len_result_arr,char init_symbol,char previous_symbol,char symbol){
    char** symbol_production;
    int len_symbol_production=0;
    if(is_in_arr(symbol,G.term,G.num_term)){
        if(symbol!='#' || is_last_symbol){
            if(!is_in_arr(symbol,visited_arr,*len_visited_arr) && !is_in_arr(symbol,result_arr,*len_result_arr)){
                visited_arr[(*len_visited_arr)++]=symbol;
                result_arr[(*len_result_arr)++]=symbol;
            }
        }
        else{
            if(previous_symbol==init_symbol || init_symbol==symbol){
                result_arr[(*len_result_arr)++]=symbol;
            }
            epsilon_found=1;
        }
        return;
    }
    else{
        symbol_production=generate_production(G,&len_symbol_production,symbol);
    }
    // printf("%d\n",len_symbol_production);
    // printf("%s\n",symbol_production[0]);
    if(len_symbol_production==0){
        printf("Invalid symbol\n");
        exit(0);
        return;
    }
    else if(is_in_arr(symbol,visited_arr,*len_visited_arr) && !is_last_symbol){
        // printf("Already Visited");
        return;
    }
    else{
        visited_arr[(*len_visited_arr)++]=symbol;
        for (int i=0;i<len_symbol_production;i++){
            has_non_terminal=0;
            go_next=0;
            if(previous_symbol=='\0'){
                is_last_symbol=0;
            }
            for(int j=0;symbol_production[i][j]!='\0';j++){
                epsilon_found=0;
                char tempsymbol=symbol_production[i][j];
                if(is_in_arr(tempsymbol,G.non_term,G.num_non_term)){
                    if(previous_symbol=='\0' && symbol_production[i][j+1]=='\0'){
                        is_last_symbol=1;
                    }
                }
                first(G,visited_arr,len_visited_arr,result_arr,len_result_arr,init_symbol,symbol,tempsymbol);
                if(go_next){
                    break;
                }

                if(is_in_arr(tempsymbol,G.non_term,G.num_non_term)){
                    has_non_terminal=1;
                }

                if(is_in_arr(tempsymbol,G.term,G.num_term) && tempsymbol!='#'){
                    if(!has_non_terminal && symbol_production[i][j+1]=='\0'){
                        go_next=1;
                    }
                    break;
                }
            }
        }
    }
}

char** find_follow_production(grammar G,int* len_follow_production,char symbol){
    int start_insert=0;
    char** result=(char**)malloc(100*sizeof(char*));
    result[0]="\0";
    for(int i=0;i<G.num_rules;i++){
        int len_temp=1;
        char* tempsymbol=(char*)malloc(10*sizeof(char));
        for(int j=0;G.production_rules[i][1][j]!='\0';j++){
            if(G.production_rules[i][1][j]==symbol){
                start_insert=1;
                continue;
            }
            if(start_insert){
                tempsymbol[len_temp]=G.production_rules[i][1][j];
                len_temp++;
            }
        }
        if(start_insert){
            tempsymbol[0]=G.production_rules[i][0][0];
            tempsymbol[len_temp]='\0';
            result[*len_follow_production]=tempsymbol;
            (*len_follow_production)++;
        }
        start_insert=0;
    }
    return result;
}

void follow(grammar G,char* visited_arr,int* len_visited_arr,char* result_arr,int* len_result_arr,char symbol){
    if(!is_in_arr(symbol,G.non_term,G.num_non_term)){
        printf("Invalid Symbol");
        exit(0);
    }
    int len_follow_production=0;
    char** follow_production=find_follow_production(G,&len_follow_production,symbol);
    if(symbol==G.start_symbol){
        result_arr[(*len_result_arr)++]='$';
    }

    if(is_in_arr(symbol,visited_arr,*len_visited_arr)){
        return;
    }
    else{
        visited_arr[(*len_visited_arr)++]=symbol;
        for(int i=0;i<len_follow_production;i++){
            for(int j=0;follow_production[i][j]!='\0';j++){
                if(j!=0 && !epsilon_found){
                    break;
                }
                if(j==0){
                    j=1;
                }
                epsilon_found=0;
                if(follow_production[i][j]=='\0'){
                    char tempsymbol=follow_production[i][0];
                    follow(G,visited_arr,len_visited_arr,result_arr,len_result_arr,tempsymbol);
                    break;
                }
                else{
                    char* temp_visited_arr=(char*)malloc(100*sizeof(char));
                    int len_temp_visited_arr=0;
                    char* first_set=(char*)malloc(100*sizeof(char));
                    int len_first_set=0;

                    char tempsymbol=follow_production[i][j];
                    first(G,temp_visited_arr,&len_temp_visited_arr,first_set,&len_first_set,tempsymbol,'\0',tempsymbol);

                    free(temp_visited_arr);
                    for(int k=0;k<len_first_set;k++){
                        if(first_set[k]!='#'){
                            if(!is_in_arr(first_set[k],result_arr,*len_result_arr)){
                                result_arr[(*len_result_arr)++]=first_set[k];
                            }
                        }
                        else{
                            if(j==len_follow_production-1){
                                temp_visited_arr=(char*)malloc(100*sizeof(char));
                                len_temp_visited_arr=0;
                                
                                char tempsymbol=follow_production[i][0];
                                follow(G,temp_visited_arr,&len_temp_visited_arr,result_arr,len_result_arr,tempsymbol);
                                
                                free(temp_visited_arr);
                            }
                            else{
                                epsilon_found=1;
                            }
                        }
                    }
                }
            }
        }
    }
}

#endif