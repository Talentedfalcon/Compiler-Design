#ifndef lr1_utils
#define lr1_utils

#include <stdio.h>
#include <stdlib.h>
#include "grammar.h"

typedef struct{
    char*** production_rules;
    int num_rules;
    int* dot_pos;
    char** lookaheads;
}lr1_item;

typedef struct{
    lr1_item i;
    char** direction;
    int num_directions;
}lr1_item_node;

//Add the production @->.{start_symbol} to the end of the grammar's production
grammar augment_grammar(grammar G){
    G.non_term[G.num_non_term++]='@';
    char* LHS=(char*)malloc(1*sizeof(char));
    LHS="@";
    char* RHS=(char*)malloc(2*sizeof(char));
    RHS[0]='.';
    RHS[1]=G.start_symbol;
    char** temp=(char**)malloc(2*sizeof(char*));
    temp[0]=LHS;
    temp[1]=RHS;
    G.production_rules[G.num_rules++]=temp;
    G.start_symbol='@';
    return G;
}

//Initialize an lr1_item with a single production,dot position and lookahead
lr1_item init_item(char** production,int dot_pos,char* lookaheads){
    lr1_item i;
    if(production[1][dot_pos]!='.'){
        printf("Invalid Production");
        exit(0);
    }
    else{
        i.production_rules=(char***)malloc(100*sizeof(char**));
        char** temp_production_rule=(char**)malloc(2*sizeof(char*));
        char* RHS=(char*)malloc(100*sizeof(char));
        char* LHS=(char*)malloc(100*sizeof(char));
        strcpy(RHS,production[0]);
        strcpy(LHS,production[1]);
        temp_production_rule[0]=RHS;
        temp_production_rule[1]=LHS;
        i.production_rules[0]=temp_production_rule;
        i.num_rules=1;
        char* temp=i.production_rules[i.num_rules-1][1];
        for(int j=0;temp[j]!='\0';j++){
            if(temp[j]=='.'){
                i.dot_pos=(int*)malloc(100*sizeof(int));
                i.dot_pos[i.num_rules-1]=j;
                break;
            }
        }
        i.lookaheads=(char**)malloc(100*sizeof(char*));
        i.lookaheads[0]=lookaheads;
    }
    return i;
}

//Join 2 items together without creating duplicates
lr1_item join_items(lr1_item i1,lr1_item i2){
    int is_unique=1;
    for(int j=0;j<i2.num_rules;j++){
        // for(int k=0;k<i1.num_rules;k++){
        //     if(compare_productions(i1.production_rules[k],i2.production_rules[j])){
        //         is_unique=0;
        //         break;
        //     }
        // }
        if(is_unique){
            i1.production_rules[i1.num_rules+j]=i2.production_rules[j];
            i1.dot_pos[i1.num_rules+j]=i2.dot_pos[j];
            i1.lookaheads[i1.num_rules+j]=i2.lookaheads[j];
        }
    }
    i1.num_rules+=i2.num_rules;
    return i1;
}

//Return the string of symbols which appear right after the dot and it's adjacent symbol for a given grammar
char* symbols_after_dot(char* production_RHS,int dot_pos){
    char* after_dot=(char*)malloc((strlen(production_RHS)-dot_pos+1)*sizeof(char));
    after_dot[0]='\0';
    // printf("%d %d",strlen(production_RHS),dot_pos);
    int i;
    for(i=dot_pos+2;i<strlen(production_RHS);i++){
        after_dot[i-dot_pos-2]=production_RHS[i];
    }
    after_dot[i-dot_pos-2]='\0';
    return after_dot;
}

char** remove_dot(char** production){
    char* temp_LHS=production[0];
    char* temp_RHS=(char*)malloc(strlen(production[1])*sizeof(char));
    int dot_found=0;
    for(int i=0;i<=strlen(production[1]);i++){
        temp_RHS[i-dot_found]=production[1][i];
        if(production[1][i]=='.'){
            dot_found=1;
            // temp_RHS[i-dot_found]='\0';
        }
    }
    char** temp_production=(char**)malloc(2*sizeof(char*));
    temp_production[0]=temp_LHS;
    temp_production[1]=temp_RHS;
    return temp_production;
}

//Compare 2 items based on it's productions and lookaheads and return True if they are equal
int compare_items(lr1_item i1,lr1_item i2){
    int equalProductions=0;
    if(i1.num_rules==i2.num_rules){
        for(int j=0;j<i1.num_rules;j++){
            if(compare_productions(i1.production_rules[j],i2.production_rules[j])){
                if(!strcmp(i1.lookaheads[j],i2.lookaheads[j])){
                    equalProductions++;
                }
            }
        }
        if(equalProductions==i1.num_rules){
            return 1;
        }
    }
    return 0;
}

//Check if an item i is already present in the item_list
int is_in_items_list(lr1_item i,lr1_item_node* item_list,int len_item_list){
    for(int j=0;j<len_item_list;j++){
        if(compare_items(i,item_list[j].i)){
            return j;
        }
    }
    return -1;
}

//Print the productions of an item along with it's lookaheads
void print_item(lr1_item i){
    for(int j=0;j<i.num_rules;j++){
        printf("\t%s -> %s,%s\n",i.production_rules[j][0],i.production_rules[j][1],i.lookaheads[j]);
    }
}

void print_item_list(lr1_item_node* item_list,int len_item_list){
    for(int i=0;i<len_item_list;i++){
        printf("Item %d:\n",i);
        print_item(item_list[i].i);
        printf("\tDirections: \n");
        if(item_list[i].num_directions==0){
            printf("\t\tReduce State\n");
        }
        else{
            for(int j=0;j<item_list[i].num_directions;j++){
                printf("\t\t%c,%d\n",item_list[i].direction[j][0],item_list[i].direction[j][1]);
            }
        }
    }
}

void lr1_closure(grammar aug_G,lr1_item* i){
    int j=0;
    while(j<i->num_rules){
        if(is_in_arr('.',i->production_rules[j][1],strlen(i->production_rules[j][1]))){
            char symbol=i->production_rules[j][1][((i->dot_pos)[j])+1];
            if(is_in_arr(symbol,aug_G.non_term,aug_G.num_non_term)){
                int len_symbol_production=0;
                char** symbol_production=generate_production(aug_G,&len_symbol_production,symbol);
                
                char* temp=(char*)malloc(2*sizeof(char));
                temp[0]=symbol;
                temp[1]='\0';
                
                char* after_dot=symbols_after_dot(i->production_rules[j][1],i->dot_pos[j]);
                // printf("%s\n",after_dot);

                char* first_set=(char*)malloc(100*sizeof(char));
                int len_first_set=0;
                for(int k=0;k<strlen(after_dot);k++){
                    char* visited_arr=(char*)malloc(100*sizeof(char));
                    int len_visited_arr=0;
                    first(aug_G,visited_arr,&len_visited_arr,first_set,&len_first_set,aug_G.production_rules[0][0][0],'\0',after_dot[k]);
                }
                for(int k=0;k<strlen(i->lookaheads[j]);k++){
                    char* visited_arr=(char*)malloc(100*sizeof(char));
                    int len_visited_arr=0;
                    if(i->lookaheads[j][k]!='$'){
                        first(aug_G,visited_arr,&len_visited_arr,first_set,&len_first_set,aug_G.production_rules[0][0][0],'\0',i->lookaheads[j][k]);
                    }
                }
                first_set[len_first_set]='\0';

                // printf("{ ");
                // for(int k=0;k<strlen(first_set);k++){
                //     printf("%c",first_set[k]);
                // }
                // printf(" }\n");

                char* lookaheads;
                if(strlen(first_set)>0){
                    lookaheads=first_set;
                }
                else{
                    lookaheads=i->lookaheads[j];
                }

                for(int k=0;k<len_symbol_production;k++){
                    char** temp_production=(char**)malloc(2*sizeof(char*));
                    temp_production[0]=temp;
                    char* temp_RHS=(char*)malloc(100*sizeof(char));
                    temp_RHS[0]='.';
                    if(symbol_production[k][0]!='#'){
                        strcat(temp_RHS,symbol_production[k]);
                    }
                    temp_production[1]=temp_RHS;

                    // printf("%s->%s,%s\n",temp_production[0],temp_production[1],lookaheads);

                    int production_in_item=0;
                    for(int l=0;l<i->num_rules;l++){
                        if((!strcmp(lookaheads,i->lookaheads[l])) && compare_productions(temp_production,i->production_rules[l])){
                            production_in_item=1;
                        }
                    }
                    if(!production_in_item){
                        i->production_rules[i->num_rules]=temp_production;
                        i->dot_pos[i->num_rules]=0;
                        i->lookaheads[i->num_rules]=lookaheads;
                        i->num_rules++;
                    }
                }
                // printf("\n");
            }
        }
        else{
            printf("Invalid Item");
            exit(0);
        }
        // printf("%d %d\n",i->num_rules,j);
        // print_item(*i);
        j++;
    }
}

lr1_item lr1_GOTO(grammar aug_G,lr1_item_node* item_list,int* len_item_list,int item_no){
    lr1_item i=item_list[item_no].i;
    char** direction=(char**)malloc(100*sizeof(char*));
    int num_directions=0;
    
    int visited_productions[i.num_rules];
    for(int k=0;k<i.num_rules;k++){
        visited_productions[k]=0;
    }

    for(int j=0;j<i.num_rules;j++){
        if(strlen(i.production_rules[j][1])==i.dot_pos[j]+1){
            visited_productions[j]=1;
            continue;
        }
        else if(!visited_productions[j]){
            char symbol_after_dot=i.production_rules[j][1][i.dot_pos[j]+1];

            int len_GOTO_items=0;
            lr1_item* GOTO_items=(lr1_item*)malloc(100*sizeof(lr1_item));

            for(int k=j;k<i.num_rules;k++){
                if(i.production_rules[k][1][i.dot_pos[k]+1]==symbol_after_dot){
                    GOTO_items[len_GOTO_items++]=init_item(i.production_rules[k],i.dot_pos[k],i.lookaheads[k]);
                    lr1_item* temp_i=&GOTO_items[len_GOTO_items-1];
                    temp_i->dot_pos[0]++;
                    temp_i->production_rules[0][1][temp_i->dot_pos[0]-1]=temp_i->production_rules[0][1][temp_i->dot_pos[0]];
                    temp_i->production_rules[0][1][temp_i->dot_pos[0]]='.';
                    lr1_closure(aug_G,temp_i);
                    visited_productions[k]=1;
                }
            }

            // printf("GOTO Item:\n");
            // for(int k=0;k<len_GOTO_items;k++){
            //     printf("\tItem %d:\n",k);
            //     print_item(GOTO_items[k]);                
            // }

            for(int k=1;k<len_GOTO_items;k++){
                GOTO_items[0]=join_items(GOTO_items[0],GOTO_items[k]);
            }

            int new_item_pos;
            if((new_item_pos=is_in_items_list(GOTO_items[0],item_list,*len_item_list))==-1){
                item_list[(*len_item_list)++].i=GOTO_items[0];
            }
            char* temp_direction=(char*)malloc(2*sizeof(char));
            temp_direction[0]=i.production_rules[j][1][i.dot_pos[j]+1];
            if(new_item_pos==-1){
                temp_direction[1]=(*len_item_list)-1;
            }
            else{
                temp_direction[1]=new_item_pos;
            }
            // printf("%c,%d\n",temp_direction[0],temp_direction[1]);
            direction[num_directions++]=temp_direction;

            // printf("Combined Items:\n");
            // print_item(GOTO_items[0]);
        }
    }
    item_list[item_no].direction=direction;
    item_list[item_no].num_directions=num_directions;
}



#endif