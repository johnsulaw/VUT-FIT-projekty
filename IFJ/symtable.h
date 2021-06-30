
#ifndef IFJ20_SYMTABLE_H
#define IFJ20_SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"

// data types enumeration

typedef enum
{
    DataTypeInt,        // int
    DataTypeDouble,     // float64
    DataTypeString,     // string
    DataTypeBool,       // bool
    DataTypeFunc,       // func
    DataTypeUndefined,
    DataTypeUnderscore  // special type for special id "_"
} DataType;

// BST node struct
typedef struct Node {
    char *key;              // key1 (id)
    DataType type;          // data type
    int arg_cnt;            // number of arguments (if function)
    int ret_cnt;            // number of return values (if function)

    DataType ret_types[20]; // array for types of function return values
    DataType arg_types[20]; // array for types of function arguments

    int declared;           // is this id declared?
    struct Node* LPtr;
    struct Node* RPtr;
} *nodePtr;

void table_init(nodePtr* root);
int table_insert_function_args(nodePtr* root, char* key, DataType arg_types[], DataType ret_types[]);
int table_insert(nodePtr* root, char* key, int arg_cnt, int ret_cnt, DataType type, int declared);
nodePtr table_search(nodePtr root, char* key);
void table_dispose(nodePtr* root);

#endif //IFJ20_SYMTABLE_H
