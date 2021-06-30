//Project: IFJ 20
//File: dynamic_string.c
//Author(s): Jan Šulavík (xsulav01)
//Date: 1. 10. 2020

#include "dynamic_string.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>

// function initializes dynamic string of 50 chars
int my_string_init(MyString* string){
    //allocate memory for initial string
    if((string->value = (char*)malloc(INITIAL_LENGTH * sizeof(char))) == NULL){
        return ERR_INTERNAL;
    }

    string->length = 0;
    string->allocated_length = INITIAL_LENGTH;

    //all 50 chars made empty chars
    for(int i = 0 ; i < INITIAL_LENGTH ; i++){
        string->value[i] = '\0';
    }

    return 0;
}

// function appends character to the end of dynamic string
int my_strncat(MyString* dst, char c){
    if(dst == NULL || dst->value == NULL){
        return 1;
    }

    // if there is not enough allocated space, allocate more
    if(dst->length + 1 >= dst->allocated_length){
        if((dst->value = (char*) realloc(dst->value, dst->length + 1 * sizeof(char))) == NULL){
            return 1;
        }

        dst->allocated_length++;
    }

    dst->value[dst->length] = c;
    dst->value[dst->length + 1] = '\0';
    dst->length++;

    return 0;

}

// function appends regular string to the end of dynamic string
int my_strcat(MyString* dst, char* src){
    int src_length = strlen(src);

    for(int i = 0; i < src_length; i++){
        my_strncat(dst, src[i]);
    }
    return 0;
}

// function empties the string
void my_strclear(MyString* str){
    for(int i = 0; i < str->length; i++){
        str->value[i] = '\0';
    }
}

// function frees the dynamic string
void my_string_free(MyString* str){
    free(str->value);
    str->value = NULL;

    str->allocated_length = 0;
    str->length = 0;

}
