//Project: IFJ 20
//File: scanner.h
//Author(s): Jan Šulavík (xsulav01)
//Date: 1. 10. 2020

#ifndef IFJ20_SCANNER_H
#define IFJ20_SCANNER_H

#include "error.h"
#include "dynamic_string.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SYMBOLS_NUMBER 15


//enum for automata states
typedef enum {
    STATE_START,                //default state

    STATE_INT,                  //END STATE

    STATE_DOUBLE_POINT,
    STATE_DOUBLE_POINT_NUM,     //END STATE
    STATE_DOUBLE_EXP,
    STATE_DOUBLE_EXP_SIGN,
    STATE_DOUBLE_EXP_NUM,       //END STATE

    STATE_COMMENT_LINE,
    STATE_COMMENT_MULTILINE,
    STATE_COMMENT_MULTILINE_END,

    STATE_ID,                   //END STATE

    STATE_STRING_START,
    STATE_STRING_END,           //END STATE
    STATE_STRING_ESCAPE,
    STATE_STRING_CHARACTER,
    STATE_STRING_HEX1,
    STATE_STRING_HEX2,

    STATE_OP_DIV,   /* / */
    STATE_OP_GT,    /* > */
    STATE_OP_EQGT,  /* >= */
    STATE_OP_LT,    /* < */
    STATE_OP_EQLT,  /* <= */
    STATE_OP_EQ,    /* == */
    STATE_OP_NEG,   /* ! */
    STATE_OP_NEQ,   /* !=*/
    STATE_OP_DEFINE,   /* := */
    STATE_OP_ASSIGN,   /* = */

    STATE_COLON,      /* : */

}State;

//enum for token types
typedef enum {
    tt_id,          //0
    tt_int,         //1
    tt_double,      //2
    tt_keyword_package, //3
    tt_string,      //4
    tt_eof,         //5
    tt_none,        //6
    tt_error,       //7
    tt_mul,         //8
    tt_add,         //9
    tt_sub,         //10
    tt_div,         //11
    tt_gt,          //12
    tt_eqgt,        //13
    tt_lt,          //14
    tt_eqlt,        //15
    tt_eq,          //16
    tt_neg,         //17
    tt_neq,         //18
    tt_define,      //19
    tt_assign,      //20
    tt_colon,       //21
    tt_comma,       //22
    tt_semicolon,   //23
    tt_left_par,    //24
    tt_right_par,   //25
    tt_left_bra,    //26
    tt_right_bra,   //27
    tt_keyword_if,      //28
    tt_keyword_else,    //29
    tt_keyword_int,     //30
    tt_keyword_float64, //31
    tt_keyword_for,     //32
    tt_keyword_func,    //33
    tt_keyword_return,  //34
    tt_keyword_string,  //35
    tt_eol              //36

}TokenType;

//struct defining token
typedef struct {
    MyString attribute;
    TokenType type;
}Token;


int is_operator_or_separator(char c);

//function to get one token from source code
Token get_token();


#endif //IFJ20_SCANNER_H
