#ifndef gen_utils
#define gen_utils

#include <stdio.h>

/*Array Functions*/
int is_in_arr(char key,char* arr,int len_arr){
    for(int i=0;i<len_arr;i++){
        if(arr[i]==key){
            return i+1;
        }
    }
    return 0;
}

void remove_from_arr(char* arr,char symbol,int* len_arr){
    int start_shift=0;
    for(int i=0;i<*len_arr;i++){
        if(arr[i]==symbol){
            start_shift=1;
        }
        if(start_shift){
            if(i!=(*len_arr)-1){
                arr[i]=arr[i+1];
            }
            else{
                arr[i]='\0';
                break;
            }
        }
    }
    (*len_arr)--;
}

/*Stack Functions*/
struct stack{
    char* arr;
    int top;
};

void push(struct stack* s,char data){
    s->top++;
    s->arr[s->top]=data;
}

void pop(struct stack* s){
    s->arr[s->top]=-1;
    s->top--;
}

char top(struct stack* s){
    return s->arr[s->top]; 
}

void display_stack(struct stack* s){
    for(int i=0;i<s->top+1;i++){
        if(i%2==0){
            printf("%c",s->arr[i]);
        }
        else{
            printf("%d",s->arr[i]);
        }        
    }
    printf("\t%d",s->top);
}

/*GOTO Action Function*/
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