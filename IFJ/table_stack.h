//
// Created by jan on 03.12.20.
//

#include "symtable.h"

#ifndef IFJ20_TABLE_STACK_H

#define MAX_STACK 1000

typedef struct TStack {
    int top;
    nodePtr* table[MAX_STACK];
}tableStack;

tableStack tableStack_create();
bool tableStack_full(tableStack* stack);
bool tableStack_empty(tableStack* stack);
bool tableStack_push(tableStack* stack, nodePtr* table);
bool tableStack_pop(tableStack* stack);


#define IFJ20_TABLE_STACK_H

#endif //IFJ20_TABLE_STACK_H
