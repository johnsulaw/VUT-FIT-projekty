//Project: IFJ 20
//File: error.c
//Author(s): Jan Šulavík (xsulav01)
//Date: 1. 10. 2020


#include "error.h"

/* exit_error function checks, if value of given return code is possible
 * on success returns desired return code */
int exit_error(int err_num){
    if(err_num == ERR_LEX){
        fprintf(stderr, "Lexical analysis error!\n");
        return err_num;
    }else if(err_num == ERR_SYN){
        fprintf(stderr, "Syntactical analysis error!\n");
        return err_num;
    }else if(err_num == ERR_SEM_BAD_DEFINITION ||
             err_num == ERR_SEM_DEDUCE_TYPE ||
             err_num == ERR_SEM_TYPE_COMP ||
             err_num == ERR_SEM_BAD_ARGS ||
             err_num == ERR_SEM_OTHERS ||
             err_num == ERR_SEM_ZERO_DIVISION){
        fprintf(stderr, "Semantic analysis error!\n");
        return err_num;
    }else if(err_num == ERR_INTERNAL){
        fprintf(stderr, "Internal program error!\n");
        return err_num;
    }else{
        fprintf(stderr, "Unknown error code!\n");
        return BAD_ERR_CODE;
    }
}