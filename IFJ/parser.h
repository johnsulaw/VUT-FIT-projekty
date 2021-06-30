//Project: IFJ 20
//File: parser.h
//Author(s): Jan Šulavík (xsulav01) , Michal Polonec (xpolon02)
//Date: 1. 10. 2020

#ifndef IFJ20_PARSER_H
#define IFJ20_PARSER_H

#include "scanner.h"
#include "error.h"
#include "symtable.h"
#include "expression.h"
#include "table_stack.h"

int parse();

// data co budu zdielane medzi pravidlami, tu si dopln aj data pre semanticku analyzu nech je to jednotne
typedef struct
{
    int argument_c; // pocet argumentov
    char *name;             //nazov premennej/funkcie

    char *assigning_name;   //nazov identifikatora, do ktoreho ideme priradit
    DataType assigning_type;// data type of variable that will be defined

    char *function;         //nazov funkcie

    int ret_values_num;     //holds number of return values for function

    char* in_func;

    Token token;
    Token prev_token;

    int prev_exists;    // existuje predosly ? - bude sa uplatnovat pri nutnosti pozriet sa na dalsi token pri nedeterminizme :sadcat:
                        // 0 - je prazdny
                        // 1 - je naplneny

    int expr_function;  //flag pro expression
                        // 0 - jde o vyrazy jako argumenty funkce
                        // 1 - nejde o ...
    int in_function;
    int if_or_for;      // pre precedencnu a semanticku
                        // 0 - nic 
                        // 1 - ak if
                        // 2 - ak for definition
                        // 3 - ak for args(necessary rule)
                        // 4 - ak for prirad(change)
    int in_for;
    char* iterator;

    int right_expr;     // 0 - on the right side of assignment is expression
                        // 1 - on the right side of assignment is function call

    tableStack table_stack;

    DataType arg_t_arr[20];
    DataType ret_t_arr[20];

}ParserData;

int table_copy(nodePtr* dest, nodePtr src);


//definicie funkcii na syntakticku analyzu
int load_token(ParserData* data);
int pociatocny_stav(ParserData *data);
int zaciatocny_stav(ParserData *data);
int definicia_funkcie(ParserData *data);
int argumenty_vstupne(ParserData *data);
int args(ParserData *data);
int argumenty_navratove(ParserData *data);
int navratove(ParserData *data);
int navratove_2(ParserData *data);
int statement_list(ParserData *data);
int statement(ParserData *data);
int lava_strana(ParserData *data);
int prava_strana(ParserData *data);
int prava_strana_2(ParserData *data);
int vyrazy(ParserData *data);
int vyrazy_next(ParserData *data);
int for_definition(ParserData *data);
int for_args(ParserData *data);
int for_prirad(ParserData *data);
int return_list(ParserData *data);
int return_statement(ParserData *data);
int type(ParserData *data);
nodePtr check_if_id_declared(nodePtr* table, char* key);


#endif //IFJ20_PARSER_H
