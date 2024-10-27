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