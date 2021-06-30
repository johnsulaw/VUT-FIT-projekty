//Project: IFJ 20
//File: error.h
//Author(s): Jan Šulavík (xsulav01)
//Date: 1. 10. 2020

#ifndef IFJ20_ERROR_H
#define IFJ20_ERROR_H

//definitions of error return codes

#include <stdio.h>

#define ERR_LEX 1 //lexical analysis error

#define ERR_SYN 2 //syntactical analysis error

#define ERR_SEM_BAD_DEFINITION 3 //semantic error for undefined/redefined variable/function
#define ERR_SEM_DEDUCE_TYPE 4 //semantic error for deduction of data type of a newly defined variable
#define ERR_SEM_TYPE_COMP 5 //semantic error for incompatible data types of variables in expressions
#define ERR_SEM_BAD_ARGS 6 //semantic error for bad number of args, bad arg types or return value types
#define ERR_SEM_OTHERS 7 //other semantic analysis errors
#define ERR_SEM_ZERO_DIVISION 9 //division by zero error

#define ERR_INTERNAL 99 //internal error

#define BAD_ERR_CODE 55 //return value of exit_error function in case it gets bad error code number

/* exit_error function checks, if value of given return code is possible
 * on success returns desired return code */
int exit_error(int err_num);

#endif //IFJ20_ERROR_H
