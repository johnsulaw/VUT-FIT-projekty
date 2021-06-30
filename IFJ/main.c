//Project: IFJ 20
//File: main.c
//Author(s): Jan Šulavík (xsulav01)
//Date: 1. 10. 2020

#include <stdio.h>
#include "error.h"
#include "expression.h"
#include "parser.h"


int main(int argc, char** argv) {
    //program runs always without arguments
    if(argc > 1){
        exit_error(ERR_INTERNAL);
        return ERR_INTERNAL;
    }

    int result = parse();

    if(result != 0){
        exit_error(result);
        fprintf(stderr ,"Compilation ended with error code: %d\n", result);
        return result;
    }else{
        fprintf(stderr, "Compilation finished successfully\n");
        return 0;
    }
}