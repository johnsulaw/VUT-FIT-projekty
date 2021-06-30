//Project: IFJ 20
//File: table_stack.c
//Author(s): Jan Šulavík (xsulav01)
//Date: 23. 11. 2020

#include "table_stack.h"

tableStack tableStack_create(){
    tableStack stack;
    stack.top = -1;

    return stack;
}

bool tableStack_full(tableStack* stack){
    return stack->top == MAX_STACK-1;
}

bool tableStack_empty(tableStack* stack){
    return stack->top == -1;
}

bool tableStack_push(tableStack* stack, nodePtr* table){
    if(tableStack_full(stack)){
        return false;
    }

    ++stack->top;
    stack->table[stack->top] = table;
    return true;
}

bool tableStack_pop(tableStack* stack){
    if(tableStack_empty(stack)){
        return false;
    }

    stack->table[stack->top] = NULL;
    stack->top--;

    return true;
}
