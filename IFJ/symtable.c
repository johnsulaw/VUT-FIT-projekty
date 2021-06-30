//Project: IFJ 20
//File: symtable.c
//Author(s): Matej Zednik, Jan Sulavik
//Date: 5. 11. 2020

#include "symtable.h"

void table_init(nodePtr* root) {
    *root = NULL;
}

void table_dispose(nodePtr* root){
    if((*root) != NULL){
        table_dispose(&(*root)->RPtr);
        table_dispose(&(*root)->LPtr);
        free((*root)->key);
        free(*root);

        *root = NULL;
    }
}

nodePtr table_search(nodePtr root, char* key){
    nodePtr node = NULL;

    if(root != NULL && strcmp(key, root->key) == 0){
        return root;
    }

    if(root != NULL){
        node = table_search(root->LPtr, key);
        if(!node) {
            node = table_search(root->RPtr, key);
        }
    }else{
        return NULL;
    }

    return node;
}

int table_insert_function_args(nodePtr* root, char* key, DataType arg_types[], DataType ret_types[]){
    if(*root == NULL){
        return 1;
    }else{
        int i;
        i = strcmp((*root)->key, key);

        if(i < 0){
            return table_insert_function_args(&(*root)->RPtr, key, arg_types, ret_types);
        }else if (i > 0){
            return table_insert_function_args(&(*root)->LPtr, key, arg_types, ret_types);
        }else{
            // we found the key
            for(int x = 0 ; x < 20 ; x++){
                (*root)->arg_types[x] = arg_types[x];
                (*root)->ret_types[x] = ret_types[x];
            }
        }

        return 0;
    }
}

int table_insert(nodePtr* root, char* key, int arg_cnt, int ret_cnt, DataType type, int declared) {
    if(*root == NULL){
        *root = malloc (sizeof(struct Node));
        if(*root == NULL){
            return ERR_INTERNAL;
        }
        (*root)->LPtr = NULL;
        (*root)->RPtr = NULL;
        (*root)->arg_cnt = arg_cnt;
        (*root)->ret_cnt = ret_cnt;
        (*root)->key = malloc(strlen(key) * sizeof(char));

        if((*root)->key == NULL){
            return ERR_INTERNAL;
        }

        strcpy((*root)->key, key);
        (*root)->type = type;
        (*root)->declared = declared;

        for(int i = 0 ; i < 20 ; i++){
            (*root)->arg_types[i] = DataTypeUndefined;
            (*root)->ret_types[i] = DataTypeUndefined;
        }

        return 0;
    }else{
        int i;
        i = strcmp((*root)->key, key);

        if(i < 0){
            return table_insert(&(*root)->RPtr, key, arg_cnt, ret_cnt, type, declared);
        }else if (i > 0){
            return table_insert(&(*root)->LPtr, key, arg_cnt, ret_cnt, type, declared);
        }else{
            strcpy((*root)->key, key);
            (*root)->type = type;

            if(type != DataTypeFunc){
                (*root)->declared = declared;
            }
            (*root)->arg_cnt = arg_cnt;
            (*root)->ret_cnt = ret_cnt;
        }

        return 0;
    }
}
