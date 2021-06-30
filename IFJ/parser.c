//Project: IFJ 20
//File: parser.c
//Author(s): Jan Šulavík (xsulav01), Michal Polonec(xpolon02)
//Date: 1. 10. 2020

#include "expression.h"
#include "parser.h"

#define NO_ARGS 0
#define NO_RETURN_VAL 0

// global symtable
nodePtr* symtable;

// function gets token from input file and saves it to parser data
int load_token(ParserData* data){
    Token token = get_token();

    // check if lexical analysis went OK
    if(token.type == tt_error){
        return ERR_LEX;
    }

    data->token = token;
    return 0;
}

// function searches the tree to find undeclared functions
// returns 0 if found undeclared function
bool find_undefined_id(nodePtr RootPtr){
    if (RootPtr == NULL)
        return false;

    if (find_undefined_id(RootPtr->LPtr))
        return true;

    if (find_undefined_id(RootPtr->RPtr))
        return true;

    if (RootPtr->declared == 0)
        return true;

    return false;
}

// function defines a function in the symtable
int define_function(nodePtr* table, char* name, int argument_count, int ret_values_count){
    nodePtr node = table_search(*table, name);

    // if key is already present in the symbol table
    if(node != NULL){
        if(node->declared != 0) return ERR_SEM_BAD_DEFINITION;  // function redeclaration -> err 3
        else{
            if(argument_count != node->arg_cnt){               // function has bad number of arguments
                return ERR_SEM_BAD_ARGS;
            }
            if(ret_values_count < node->ret_cnt){             // function has bad number of return values
                return ERR_SEM_BAD_ARGS;
            }

            node->declared = 1;
            return 0;
        }
    }

    int result = table_insert(table, name, argument_count, ret_values_count, DataTypeFunc, 1);
    if(result != 0) return result;

    return 0;
}

int create_localtable(ParserData *data){
    nodePtr* local_symtable;

    local_symtable = malloc(sizeof(struct Node));
    if(local_symtable == NULL){
        return ERR_INTERNAL;
    }

    // push the table onto the stack
    if((tableStack_push(&data->table_stack, local_symtable) != 1)) return ERR_INTERNAL;

    // load the table with values from greater scope
    // if stack is empty, copy global table to it
    if(data->table_stack.top == 0){
        table_copy(data->table_stack.table[data->table_stack.top], *symtable);
    }else{  // else copy the previous table on the stack
        table_copy(data->table_stack.table[data->table_stack.top], *data->table_stack.table[data->table_stack.top - 1]);
    }

    return 0;
}

// makes a copy of table src to dest
int table_copy(nodePtr* dest, nodePtr src){
    if(src == NULL){
        return 1;
    }

    table_copy(dest, src->LPtr);
    table_copy(dest, src->RPtr);

    if(src->type == DataTypeFunc && src->declared == 1){
        return define_function(dest, src->key, src->arg_cnt, src->ret_cnt);
    }else{
        return table_insert(dest, src->key, src->arg_cnt, src->ret_cnt, src->type, 1);
    }
}

// function converts token type to DataType to save to symtable
DataType get_type(Token* arg_type_token){
    DataType type;

    if(arg_type_token->type == tt_keyword_int){
        type = DataTypeInt;
    }else if(arg_type_token->type == tt_keyword_float64){
        type = DataTypeDouble;
    }else if(arg_type_token->type == tt_keyword_string){
        type = DataTypeString;
    }else{
        return DataTypeUndefined;
    }

    return type;
}

int predefined_insert_args(ParserData* data, char* func){
    if(strcmp(func, "inputs") == 0){
        data->ret_t_arr[0] = DataTypeString;
        data->ret_t_arr[1] = DataTypeInt;

        table_insert_function_args(symtable, func, data->arg_t_arr, data->ret_t_arr);
    }else if(strcmp(func, "inputi") == 0){
        data->ret_t_arr[0] = DataTypeInt;
        data->ret_t_arr[1] = DataTypeInt;

        table_insert_function_args(symtable, func, data->arg_t_arr, data->ret_t_arr);
    }else if(strcmp(func, "inputf") == 0){
        data->ret_t_arr[0] = DataTypeDouble;
        data->ret_t_arr[1] = DataTypeInt;

        table_insert_function_args(symtable, func, data->arg_t_arr, data->ret_t_arr);
    }else if(strcmp(func, "print") == 0){
        table_insert_function_args(symtable, func, data->arg_t_arr, data->ret_t_arr);
    }else if(strcmp(func, "int2float") == 0){
        data->arg_t_arr[0] = DataTypeInt;

        data->ret_t_arr[0] = DataTypeDouble;

        table_insert_function_args(symtable, func, data->arg_t_arr, data->ret_t_arr);
    }else if(strcmp(func, "float2int") == 0){
        data->arg_t_arr[0] = DataTypeDouble;

        data->ret_t_arr[0] = DataTypeInt;

        table_insert_function_args(symtable, func, data->arg_t_arr, data->ret_t_arr);
    }else if(strcmp(func, "len") == 0){
        data->arg_t_arr[0] = DataTypeString;

        data->ret_t_arr[0] = DataTypeInt;

        table_insert_function_args(symtable, func, data->arg_t_arr, data->ret_t_arr);
    }else if(strcmp(func, "substr") == 0){
        data->arg_t_arr[0] = DataTypeString;
        data->arg_t_arr[1] = DataTypeInt;
        data->arg_t_arr[2] = DataTypeInt;

        data->ret_t_arr[0] = DataTypeString;
        data->ret_t_arr[1] = DataTypeInt;

        table_insert_function_args(symtable, func, data->arg_t_arr, data->ret_t_arr);
    }else if(strcmp(func, "ord") == 0){
        data->arg_t_arr[0] = DataTypeString;
        data->arg_t_arr[1] = DataTypeInt;

        data->ret_t_arr[0] = DataTypeInt;
        data->ret_t_arr[1] = DataTypeInt;

        table_insert_function_args(symtable, func, data->arg_t_arr, data->ret_t_arr);
    }else if(strcmp(func, "chr") == 0){
        data->arg_t_arr[0] = DataTypeInt;

        data->ret_t_arr[0] = DataTypeString;
        data->ret_t_arr[1] = DataTypeInt;

        table_insert_function_args(symtable, func, data->arg_t_arr, data->ret_t_arr);
    }else return 1;

    // reset the arrays after each use
    for(int i = 0 ; i < 20 ; i++){
        data->arg_t_arr[i] = DataTypeUndefined;
        data->ret_t_arr[i] = DataTypeUndefined;
    }
    return 0;
}

// function checks if an id with name 'key' is present in table
// if present returns node, if not returns NULL
nodePtr check_if_id_declared(nodePtr* table, char* key){
    nodePtr node = table_search(*table, key);
    return node;
}

// start parsing
int parse(){
    int result;

    symtable = malloc(sizeof(struct Node));
    if(symtable == NULL){
        return ERR_INTERNAL;
    }

    table_init(symtable);

    // struct to hold data for parsing
    ParserData data;
    data.name = NULL;
    data.prev_exists = 0;
    data.in_function = 0;
    data.right_expr = 0;
    data.argument_c = 0;
    data.ret_values_num  = 0;
    data.expr_function = 0;
    data.table_stack = tableStack_create();

    for(int i = 0 ; i < 20 ; i++){
        data.arg_t_arr[i] = DataTypeUndefined;
        data.ret_t_arr[i] = DataTypeUndefined;
    }

    // IFJ20 predefined functions
    if(((result = table_insert(symtable, "inputs", 0, 2,DataTypeFunc, 1)) != 0)) return result;
    if(((result = table_insert(symtable, "inputi", 0, 2,DataTypeFunc, 1)) != 0)) return result;
    if(((result = table_insert(symtable, "inputf", 0, 2,DataTypeFunc, 1)) != 0)) return result;
    if(((result = table_insert(symtable, "print", 20, 0,DataTypeFunc, 1)) != 0)) return result;
    if(((result = table_insert(symtable, "int2float", 1, 1,DataTypeFunc, 1)) != 0)) return result;
    if(((result = table_insert(symtable, "float2int", 1, 1,DataTypeFunc, 1)) != 0)) return result;
    if(((result = table_insert(symtable, "len", 1, 1,DataTypeFunc, 1)) != 0)) return result;
    if(((result = table_insert(symtable, "substr", 3, 2,DataTypeFunc, 1)) != 0)) return result;
    if(((result = table_insert(symtable, "ord", 2, 2,DataTypeFunc, 1)) != 0)) return result;
    if(((result = table_insert(symtable, "chr", 1, 2,DataTypeFunc, 1)) != 0)) return result;

    // load arguments and return types to functions
    predefined_insert_args(&data, "inputs");
    predefined_insert_args(&data, "inputi");
    predefined_insert_args(&data, "inputf");
    predefined_insert_args(&data, "print");
    predefined_insert_args(&data, "int2float");
    predefined_insert_args(&data, "float2int");
    predefined_insert_args(&data, "len");
    predefined_insert_args(&data, "substr");
    predefined_insert_args(&data, "ord");
    predefined_insert_args(&data, "chr");

    // insert special variable _ to global table
    table_insert(symtable, "_", NO_ARGS, NO_RETURN_VAL, DataTypeUnderscore, 1);

    // loading of first token went OK
    if((result = load_token(&data)) == 0){
        result = pociatocny_stav(&data);        //call first non-terminal

        // check if function main() is present
        if((result == 0) && (check_if_id_declared(symtable, "main") == NULL)){
            result = ERR_SEM_BAD_DEFINITION;
        }

        // check for undeclared but used functions, if result is OK
        if(result == 0){
            if(find_undefined_id(*symtable)) return ERR_SEM_BAD_DEFINITION;
        }

        table_dispose(symtable);
        free(symtable);
        return result;
    }else{
        return result;
    }
}

//-------------------Zaciatok syntaktickej analyzy------------------------//

//POCIATOCNY_STAV-> eof
//POCIATOCNY_STAV-> package main ZACIATOCNY_STAV
int pociatocny_stav(ParserData *data)
{
    int result = ERR_SYN;
        
    if(data->token.type == tt_eof)                                // POCIATOCNY_STAV-> eof
    {
        return 0;
    }else if(data->token.type == tt_keyword_package)
    {                                                            //POCIATOCNY_STAV-> package main ZACIATOCNY_STAV
        if((result = load_token(data)) != 0) return result;      // load token

        if(data->token.type == tt_id){
            if((strcmp(data->token.attribute.value, "main") == 0))
            {
                if((result = load_token(data)) != 0) return result; // load token
                if((result = zaciatocny_stav(data)) != 0 ) return result; // use next rule
                return 0;
            }else{
                return ERR_SYN;
            }            
        }
    }else{
       if(data->token.type == tt_eol){
           if((result = load_token(data)) != 0) return result; // load next token
           if((result = pociatocny_stav(data)) != 0 ) return result; // use next rule
           return 0;
       }else return ERR_SYN;
    }

    return ERR_SYN;
}

//ZACIATOCNY_STAV-> DEFINICIA_FUNKCIE ZACIATOCNY_STAV
//ZACIATOCNY_STAV-> eof
int zaciatocny_stav(ParserData *data)
{
    int result;
    
    if(data->token.type == tt_eof)
    {
        return 0;
    }else if (data->token.type == tt_keyword_func)
    {
        if((result = load_token(data)) != 0) return result; // load next token
        if((result = definicia_funkcie(data)) != 0 ) return result; // use next rule
        if((result = zaciatocny_stav(data)) != 0 ) return result; // use next rule
        return 0;
    }else if(data->token.type == tt_eol) {

        if ((result = load_token(data)) != 0) return result; // load next token
        if ((result = zaciatocny_stav(data)) != 0) return result; // use next rule
        return 0;
    }
    return ERR_SYN;
}

//DEFINICIA_FUNKCIE-> func id ( ARGUMENTY_VSTUPNE ARGUMENTY_NAVRATOVE eol STATEMENT_LIST
int definicia_funkcie(ParserData *data)
{
    int result;
    data->in_function++; // probably can use this for semantical analysis 

    if(data->token.type == tt_id)
    {

        // initialize local symtable and copy global symbols
        create_localtable(data);

        // save function name to parser data
        data->function = data->token.attribute.value;
        data->in_func = data->token.attribute.value;

        if((result = load_token(data)) != 0) return result; // load next token
        if(data->token.type != tt_left_par) return ERR_SYN;

        if((result = load_token(data)) != 0) return result; // load next token and use rules
        if((result = argumenty_vstupne(data)) != 0 ) return result; // use next rule

        if(data->token.type != tt_right_par) return ERR_SYN;

        if((result = load_token(data)) != 0) return result; // load next token and use rules
        if((result = argumenty_navratove(data)) != 0 ) return result; // use next rule

        // save function to symtable
        if((result = define_function(symtable, data->function, data->argument_c, data->ret_values_num)) != 0) return result;
        table_insert_function_args(symtable, data->function, data->arg_t_arr, data->ret_t_arr);


        // reset data
        data->argument_c = 0;
        data->ret_values_num = 0;
        data->function = NULL;

        // reset arrays
        for(int i = 0 ; i < 20 ; i++){
            data->arg_t_arr[i] = DataTypeUndefined;
            data->ret_t_arr[i] = DataTypeUndefined;
        }

        if(data->token.type != tt_eol) return ERR_SYN;

        if((result = statement_list(data)) != 0 ) return result; // use next rule

        // EXITING LOCAL SCOPE
        tableStack_pop(&data->table_stack);

        return 0;
    }

    return ERR_SYN;
}

//ARGUMENTY_VSTUPNE-> )
//ARGUMENTY_VSTUPNE-> id TYPE ARGS
int argumenty_vstupne(ParserData *data)
{
    int result;

    // main cannot have any parameters
    if((strcmp(data->function, "main") == 0) && (data->token.type != tt_right_par)) return ERR_SEM_BAD_ARGS;

    if(data->token.type == tt_right_par)
    {
        return 0;
    }else if (data->token.type == tt_id)
    {
        // add argument count
        data->argument_c++;
        data->name = data->token.attribute.value;

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = type(data)) != 0 ) return result; // use next rule // je to: id TYPE

        // convert token type to DataType of variable
        DataType arg_type = get_type(&data->token);
        if(arg_type == DataTypeUndefined) return ERR_SEM_DEDUCE_TYPE;
        data->arg_t_arr[data->argument_c - 1] = arg_type;

        // check if argument id isn't already in symtable
        if(check_if_id_declared(data->table_stack.table[data->table_stack.top], data->name) != NULL) return ERR_SEM_BAD_DEFINITION;
        table_insert(data->table_stack.table[data->table_stack.top], data->name, NO_ARGS, NO_RETURN_VAL, arg_type, 1);

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = args(data)) != 0 ) return result; // use next rule // je to: id TYPE ARGS

        return 0;
    }
    return ERR_SYN;
}

//ARGS-> , id TYPE ARGS
//ARGS-> )
int args(ParserData *data)
{
    int result;

    if(data->token.type == tt_comma)
    {
        if((result = load_token(data)) != 0) return result; // load next token
        if(data->token.type != tt_id) return ERR_SYN;

        // add argument count
        data->argument_c++;
        data->name = data->token.attribute.value;

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = type(data)) != 0 ) return result; // use next rule // je to: , id TYPE

        // convert token type to DataType of variable
        DataType arg_type = get_type(&data->token);
        if(arg_type == DataTypeUndefined) return ERR_SEM_DEDUCE_TYPE;
        data->arg_t_arr[data->argument_c - 1] = arg_type;

        // check if argument id isn't already in symtable
        if(check_if_id_declared(data->table_stack.table[data->table_stack.top], data->name) != NULL) return ERR_SEM_BAD_DEFINITION;
        table_insert(data->table_stack.table[data->table_stack.top], data->name, NO_ARGS, NO_RETURN_VAL, arg_type, 1);

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = args(data)) != 0 ) return result; // use next rule

        return 0;

    }else if (data->token.type == tt_right_par)
    {
        return 0;
    }

    return ERR_SYN;
}

//ARGUMENTY_NAVRATOVE-> ( TYPE NAVRATOVE {
//ARGUMENTY_NAVRATOVE-> ( NAVRATOVE_2 {
//ARGUMENTY_NAVRATOVE-> {
int argumenty_navratove(ParserData *data)
{
    int result;

    if(data->token.type == tt_left_par)         // je to: (
    {
        //data->prev_exists = 1;
        //data->prev_token = data->token;
        if((result = load_token(data)) != 0) return result; // load next token

        // main cannot have any return values
        if((strcmp(data->function, "main") == 0) && (data->token.type != tt_right_par)) return ERR_SEM_BAD_ARGS;


        if((data->token.type == tt_keyword_float64) ||
            (data->token.type == tt_keyword_int)    ||
            (data->token.type == tt_keyword_string))
        {
            data->ret_values_num++;

            // convert token type to DataType
            DataType ret_type = get_type(&data->token);
            if(ret_type == DataTypeUndefined) return ERR_SEM_DEDUCE_TYPE;
            data->ret_t_arr[data->ret_values_num - 1] = ret_type;

            if((result = load_token(data)) != 0) return result; // load next token
            if((result = navratove(data)) != 0 ) return result; // use next rule //je to: ( TYPE
            
            if(data->token.type != tt_left_bra) return ERR_SYN;        //je to: ( TYPE NAVRATOVE {
            if((result = load_token(data)) != 0) return result;        // load next token

            return 0;
        }else
        {
            if((result = navratove_2(data)) != 0 ) return result; // use next rule //je to: ( NAVRATOVE_2

            if(data->token.type != tt_left_bra) return ERR_SYN;        //je to: ( NAVRATOVE_2 {
            if((result = load_token(data)) != 0) return result;        // load next token

            return 0;
        }

    }else if (data->token.type == tt_left_bra)  // je to: {
    {
        if((result = load_token(data)) != 0) return result;
        return 0;
    }    
    
    return ERR_SYN;
}

//NAVRATOVE-> )
//NAVRATOVE-> , TYPE NAVRATOVE
int navratove(ParserData *data)
{
    int result;

    if(data->token.type == tt_comma)
    {
        if((result = load_token(data)) != 0) return result; // load next token
        if((data->token.type == tt_keyword_float64) ||
            (data->token.type == tt_keyword_int)    ||
            (data->token.type == tt_keyword_string))
        {
            data->ret_values_num++;                             // je to: , TYPE

            // convert token type to DataType
            DataType ret_type = get_type(&data->token);
            if(ret_type == DataTypeUndefined) return ERR_SEM_DEDUCE_TYPE;
            data->ret_t_arr[data->ret_values_num - 1] = ret_type;

            if((result = load_token(data)) != 0) return result; // load next token
            if((result = navratove(data)) != 0 ) return result; // use next rule //je to: , TYPE NAVRATOVE

            return 0;
        }else
        {
            return ERR_SYN;                         // je to: ERR_SYN
        }
        
    }else if (data->token.type == tt_right_par)
    {                                               // je to: )
        if((result = load_token(data)) != 0) return result; // load next token
        return 0;
    }

    return ERR_SYN;
}

//NAVRATOVE_2-> )
int navratove_2(ParserData *data)
{
    int result;

    if(data->token.type == tt_right_par)
    {                       // je to: )
        if((result = load_token(data)) != 0) return result; // load next token
        return 0;
    }

    return ERR_SYN;
}

//STATEMENT_LIST-> }
//STATEMENT_LIST-> STATEMENT STATEMENT_LIST
int statement_list(ParserData *data)
{
    int result = ERR_SYN;

    if(data->token.type == tt_right_bra)    //je to: }
    {
        if((result = load_token(data)) != 0) return result; // load next token

        return 0;
    }else if(data->token.type == tt_keyword_if    ||
            data->token.type == tt_eol            ||
            data->token.type == tt_id             ||
            data->token.type == tt_keyword_for    ||
            data->token.type == tt_keyword_return)
    {
        if((result = statement(data)) != 0 ) return result; // use next rule
        if((result = statement_list(data)) != 0 ) return result; // use next rule

        return 0;
    }
    
    return result;
}

//STATEMENT-> if EXPR { eol STATEMENT_LIST else { eol STATEMENT_LIST eol
//STATEMENT-> eol
//STATEMENT-> id := EXPR eol
//STATEMENT-> id LAVA_STRANA PRAVA_STRANA eol
//STATEMENT-> for FOR_DEFINITION FOR_ARGS FOR_PRIRAD eol STATEMENT_LIST
//STATEMENT-> return RETURN_LIST
//STATEMENT-> id ( VYRAZY eol
// takze za { musi byt eol avsak potom na dalsom riadku moze byt uz hned } avsak u mna zatial
// sa da iba tak ze aspon jeden volny riadok musi byt toto treba niako osefovat 
int statement(ParserData *data)
{
    int result = ERR_SYN;

    if(data->token.type == tt_keyword_if)   //je to: if
    {
        // LOAD FIRST TOKEN OF EXPRESSION ANALYSIS
        if((result = load_token(data)) != 0) return result;
        // expression cannot be empty
        if(data->token.type == tt_left_bra) return ERR_SYN;

        data->prev_exists = 0;
        data->if_or_for = 1;                // pre precedencnu

        if((result = expression(data)) != 0) return result;

        data->if_or_for = 0;
        data->prev_exists = 0;

        if(data->token.type != tt_left_bra) return ERR_SYN;

        // ENTERING 'if' SCOPE
        create_localtable(data);

        if((result = load_token(data)) != 0) return result; // load next token
        if(data->token.type != tt_eol) return ERR_SYN;

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = statement_list(data)) != 0 ) return result; // use next rule

        if(data->token.type != tt_keyword_else) return ERR_SYN;

        // EXITING 'if' SCOPE
        tableStack_pop(&data->table_stack);

        if((result = load_token(data)) != 0) return result; // load next token
        if(data->token.type != tt_left_bra) return ERR_SYN;

        // ENTERING 'else' SCOPE
        create_localtable(data);

        if((result = load_token(data)) != 0) return result; // load next token
        if(data->token.type != tt_eol) return ERR_SYN;

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = statement_list(data)) != 0 ) return result; // use next rule

        if(data->token.type != tt_eol) return ERR_SYN;

        // EXITING 'else' SCOPE
        tableStack_pop(&data->table_stack);

        return 0;

    }else if(data->token.type == tt_keyword_for)    //je to: for
    {
        // ENTERING NEW SCOPE
        create_localtable(data);
        data->in_for = 1;

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = for_definition(data)) != 0 ) return result; // use next rule //je to: for FOR_DEFINITION

        if((result = for_args(data)) != 0 ) return result; // use next rule //je to: for FOR_DEFINITION FOR_ARGS

        if((result = for_prirad(data)) != 0 ) return result; // use next rule //je to: for FOR_DEFINITION FOR_ARGS FOR_PRIRAD

        if((result = load_token(data)) != 0) return result; // load next token
        if(data->token.type != tt_eol) return ERR_SYN;

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = statement_list(data)) != 0 ) return result; // use next rule

        data->in_for = 0;
        // EXITING FOR SCOPE
        tableStack_pop(&data->table_stack);

        return 0;

    }else if (data->token.type == tt_keyword_return) //je to: return
    {
        if((result = load_token(data)) != 0) return result; // load next token
        if((result = return_list(data)) != 0 ) return result; // use next rule

        return 0;

    }else if (data->token.type == tt_eol)           //je to: eol
    {
        if((result = load_token(data)) != 0) return result; // load next token

        return 0;
    
    }else if (data->token.type == tt_id)    // moze to byt: LAVA_STRANA PRAVA_STRANA eol  alebo  id := EXPR
    {
        // save current token with id
        data->prev_token = data->token;
        data->prev_exists = 1;

        if((result = load_token(data)) != 0) return result; // load next token

        if (data->token.type == tt_define)  //je to: definicia premennej
        {
            // save variable name for assigning
            data->assigning_name = data->prev_token.attribute.value;
            // cannot redefine a variable in scope
            nodePtr id = check_if_id_declared(data->table_stack.table[data->table_stack.top], data->assigning_name);
            // if id is defined in this scope and is not a function
            if(id != NULL) {
                if (!((strcmp(data->iterator, id->key) == 0) && (data->in_for == 1))) {
                    if ((id != NULL) && (id->type != DataTypeFunc))return ERR_SEM_BAD_DEFINITION;
                }
            }

            data->prev_exists = 0;          // tu som si nie isty ci to treba

            // LOAD FIRST TOKEN OF EXPRESSION ANALYSIS
            if((result = load_token(data)) != 0) return result; // load next token
            // right side of definition cannot be empty
            if(data->token.type == tt_eol) return ERR_SYN;

            // cannot define variable by function call
            if(data->token.type == tt_id){
                id = table_search(*data->table_stack.table[data->table_stack.top], data->token.attribute.value);
                if(id != NULL){
                    if(id->type == DataTypeFunc) return ERR_SYN;
                }else{
                    // function might be not declared yet, we neet to look for '('
                    data->prev_token = data->token;
                    data->prev_exists = 1;

                    if((result = load_token(data)) != 0) return result; // load next token
                    if(data->token.type == tt_left_par) return ERR_SYN;
                }
            }

            if((result = expression(data)) != 0) return result;

            data->prev_exists = 0;

            // save variable to symtable of local scope
            table_insert(data->table_stack.table[data->table_stack.top], data->assigning_name, NO_ARGS, NO_RETURN_VAL, data->assigning_type, 1);

            if(data->token.type != tt_eol) return ERR_SYN;

            if((result = load_token(data)) != 0) return result; // load next token
            return 0;
        
        }else if((data->token.type == tt_assign) ||
                (data->token.type == tt_comma)) //tu je to priradenie do premennej/premennych
        {
            // variable has to be defined already in order to assign to it
            nodePtr id;
            if((id = check_if_id_declared(data->table_stack.table[data->table_stack.top], data->prev_token.attribute.value)) == NULL) return ERR_SEM_BAD_DEFINITION;

            // function call cannot be on the left side here
            if(id->type == DataTypeFunc) return ERR_SYN;

            // increase return values number, in case function call comes later
            // and save id type to compare with expected return type later
            data->ret_values_num++;
            data->ret_t_arr[data->ret_values_num - 1] = id->type;   //

            if((result = lava_strana(data)) != 0 ) return result; // use next rule

            data->if_or_for = 0;

            if((result = prava_strana(data)) != 0 ) return result; // use next rule

            // check if all variables on the left side got its expression
            if(data->right_expr != 1 ) {
                if (data->ret_values_num != data->argument_c) return ERR_SEM_OTHERS;
            }else{
                // for function calls
                nodePtr function = table_search(*symtable, data->function);
                if(function->ret_cnt != data->ret_values_num) return ERR_SEM_BAD_ARGS;
            }

            data->if_or_for = 0;
            data->ret_values_num = 0;
            data->argument_c = 0;
            data->expr_function = 0;
            data->right_expr = 0;

            if(data->token.type != tt_eol) return ERR_SYN;             //je to: id LAVA_STRANA PRAVA_STRANA

            if((result = load_token(data)) != 0) return result; // load next token
            return 0;
        }else if (data->token.type == tt_left_par) //je to : id ( VYRAZY eol
        {
            // get function record
            nodePtr node = table_search(*data->table_stack.table[data->table_stack.top], data->prev_token.attribute.value);

            data->function = data->prev_token.attribute.value;

            // check if id is function id
            if(node != NULL){
                if(node->type != DataTypeFunc) return ERR_SEM_BAD_DEFINITION;
            }

            if((result = load_token(data)) != 0) return result; // load next token
            if((result = vyrazy(data)) != 0 ) return result; // use next rule

            // if function was not found, predeclare it
            if(node == NULL) {
                define_function(symtable, data->function, data->argument_c, data->ret_values_num);
                table_insert_function_args(symtable, data->function, data->arg_t_arr, data->ret_t_arr);

                // reset arrays
                for(int i = 0 ; i < 20 ; i++){
                    data->arg_t_arr[i] = DataTypeUndefined;
                    data->ret_t_arr[i] = DataTypeUndefined;
                }
            }else{
                // else test correct number of arguments and return values
                // print can have arbitrary number of arguments, so dont test
                if(strcmp(node->key, "print") != 0){
                    if(node->arg_cnt != data->argument_c) return ERR_SEM_BAD_ARGS;
                }
                if(node->ret_cnt != data->ret_values_num) return ERR_SEM_BAD_ARGS;
            }

            data->argument_c = 0;
            data->ret_values_num = 0;
            data->right_expr = 0;

            return 0;
        } else return ERR_SYN; // variable cannot be on the line by itself
        
    }

    return result;
}

//LAVA_STRANA-> , id LAVA_STRANA
//LAVA_STRANA-> =
int lava_strana(ParserData *data)
{
    int result;

    if(data->token.type == tt_comma)
    {
        if((result = load_token(data)) != 0) return result; // load next token

        // check if token is id and if it is declared already
        if(data->token.type != tt_id) return ERR_SYN;
        if(check_if_id_declared(data->table_stack.table[data->table_stack.top], data->token.attribute.value) == 0) return ERR_SEM_BAD_DEFINITION;

        nodePtr id = table_search(*data->table_stack.table[data->table_stack.top], data->token.attribute.value);
        DataType ret_type = id->type;
        // function call cannot be on the left side here
        if(ret_type == DataTypeFunc) return ERR_SYN;

        data->ret_values_num++;
        data->ret_t_arr[data->ret_values_num - 1] = ret_type;

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = lava_strana(data)) != 0 ) return result; // use next rule

        return 0;
    }else
    {
        if(data->token.type != tt_assign) return ERR_SYN;

        if((result = load_token(data)) != 0) return result; // load next token
        return 0;
    }
}

//PRAVA_STRANA-> EXPR PRAVA_STRANA2
//PRAVA_STRANA-> id ( VYRAZY 
int prava_strana(ParserData *data)
{
    int result;

    data->prev_token = data->token;

    // need to look at next token, if it is '('
    if((result = load_token(data)) != 0) return result;


    if (data->token.type == tt_left_par)
    {
        // get function record
        nodePtr func = table_search(*symtable, data->prev_token.attribute.value);
        data->function = data->prev_token.attribute.value;

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = vyrazy(data)) != 0 ) return result; // use next rule

        // if function was not found, predeclare it
        if(func == NULL) {
            table_insert(symtable, data->prev_token.attribute.value, data->argument_c, data->ret_values_num, DataTypeFunc, 0);
            table_insert_function_args(symtable, data->function, data->arg_t_arr, data->ret_t_arr);

            // reset arrays
            for(int i = 0 ; i < 20 ; i++){
                data->arg_t_arr[i] = DataTypeUndefined;
                data->ret_t_arr[i] = DataTypeUndefined;
            }
        }else{
            // else test correct number of arguments
            if(func->arg_cnt != data->argument_c) return ERR_SEM_BAD_ARGS;
            for (int i = 0; i < data->ret_values_num; i++) {
                if(data->ret_t_arr[i] != DataTypeUnderscore) {
                    if (data->ret_t_arr[i] != func->ret_types[i]) return ERR_SEM_BAD_ARGS;
                }
            }
        }


        return 0;
    }else
    {
        data->prev_exists = 1;
        data->argument_c++;

        // right side of assignmment cannot be empty
        if(data->prev_token.type == tt_eol) return ERR_SYN;

        if((result = expression(data)) != 0) return result;

        // compare expression result data type to data type of variable
        if(data->ret_t_arr[data->argument_c - 1] == DataTypeUndefined) return ERR_SEM_OTHERS;
        if(data->ret_t_arr[data->argument_c - 1] != DataTypeUnderscore) {
            // do not compare with underscore
            if (data->assigning_type != data->ret_t_arr[data->argument_c - 1]) return ERR_SEM_OTHERS;
        }

        data->prev_exists = 0;

        if((result = prava_strana_2(data)) != 0 ) return result; // use next rule

        return 0;
    }
}

//PRAVA_STRANA2-> , EXPR PRAVA_STRANA2
//PRAVA_STRANA2-> epsilon                //tu je to pretoze sa to pouziva aj vo FOR_PRIRAD a tam nesmie byt eol
int prava_strana_2(ParserData *data)
{
    int result;

    if (data->token.type == tt_comma)
    {
        data->prev_exists = 0;
        data->argument_c++;

        if((result = load_token(data)) != 0) return result; // load next token

        if((result = expression(data)) != 0) return result;

        // compare expression result data type to data type of variable
        if(data->ret_t_arr[data->argument_c - 1] == DataTypeUndefined) return ERR_SEM_OTHERS;
        if(data->ret_t_arr[data->argument_c - 1] != DataTypeUnderscore) {
            // do not compare with underscore
            if (data->assigning_type != data->ret_t_arr[data->argument_c - 1]) return ERR_SEM_OTHERS;
        }

        data->prev_exists = 0;

        if((result = prava_strana_2(data)) != 0 ) return result; // use next rule

        return 0;  
    }else
    {
        return 0;
    }
}

//VYRAZY-> EXPR VYRAZY_NEXT
//VYRAZY-> )
int vyrazy(ParserData *data)
{
    int result;

    if (data->token.type == tt_right_par)
    {
        data->right_expr = 1;
        if((result = load_token(data)) != 0) return result; // load next token
        return 0;        
    }else
    {
        data->right_expr = 1;
        data->expr_function = 1;
        data->prev_exists = 0;

        if((result = expression(data)) != 0) return result;

        data->prev_exists = 0;

        // add to argument count for function calls
        data->argument_c++;

        nodePtr func = table_search(*symtable, data->function);
        // if function is not declared yet save data type of argument
        // to array for later function pre-definition
        if(func == NULL){
            data->arg_t_arr[data->argument_c - 1] = data->assigning_type;
        }else{
            // if function is already present in the table
            // data type of this expression should match expected data type saved in the table
            // do not test for print, as it can have any type as any argument
            if(strcmp(func->key, "print") != 0){
                if(func->arg_types[data->argument_c - 1] != data->assigning_type) return ERR_SEM_BAD_ARGS;
            }
        }

        if((result = vyrazy_next(data)) != 0 ) return result; // use next rule

        return 0;
    }
}

//VYRAZY_NEXT-> , EXPR VYRAZY_NEXT
//VYRAZY_NEXT-> )
int vyrazy_next(ParserData *data)
{
    int result;

    if (data->token.type == tt_eol)
    {
        return 0;
    }else
    {
        if(data->token.type != tt_comma) return ERR_SYN;
        if((result = load_token(data)) != 0) return result; // load next token

        data->expr_function = 1;
        data->prev_exists = 0;

        if((result = expression(data)) != 0) return result;

        data->prev_exists = 0;
        data->argument_c++;

        nodePtr func = table_search(*symtable, data->function);
        // if function is not declared yet save data type of argument
        // to array for later function pre-definition
        if(func == NULL){
            data->arg_t_arr[data->argument_c - 1] = data->assigning_type;
        }else{
            // if function is already present in the table
            // data type of this expression should match expected data type saved in the table
            if(strcmp(func->key, "print") != 0){
                if(func->arg_types[data->argument_c - 1] != data->assigning_type) return ERR_SEM_BAD_ARGS;
            }
        }

        if((result = vyrazy_next(data)) != 0 ) return result; // use next rule

        return 0;
    }
}

//FOR_DEFINITION-> id := EXPR ;
//FOR_DEFINITION-> ;
int for_definition(ParserData *data)
{
    int result;

    if (data->token.type == tt_id)
    {
        // save id name to data
        data->assigning_name = data->token.attribute.value;

        if((result = load_token(data)) != 0) return result; // load next token
        if(data->token.type != tt_define) return ERR_SYN;

        // LOAD FIRST TOKEN OF EXPRESSION ANALYSIS
        if((result = load_token(data)) != 0) return result; // load next token

        data->prev_exists = 0;
        data->if_or_for = 2;

        if((result = expression(data)) != 0) return result;

        table_insert(data->table_stack.table[data->table_stack.top], data->assigning_name, NO_ARGS, NO_RETURN_VAL, data->assigning_type, 1);
        data->iterator = data->assigning_name;

        data->prev_exists = 0;
        data->if_or_for = 0;

        if(data->token.type != tt_semicolon) return ERR_SYN;

        if((result = load_token(data)) != 0) return result; // load next token

        return 0;

    }else
    {
        if(data->token.type != tt_semicolon) return ERR_SYN;
        if((result = load_token(data)) != 0) return result; // load next token

        return 0;
    }
}

//FOR_ARGS-> EXPR ;
int for_args(ParserData *data)
{
    int result;

    data->prev_exists = 0;
    data->if_or_for = 3;

    if(data->token.type == tt_semicolon) return ERR_SYN;

    if((result = expression(data)) != 0) return result;

    data->prev_exists = 0;
    data->if_or_for = 0;

    if(data->token.type != tt_semicolon) return ERR_SYN;

    if((result = load_token(data)) != 0) return result; // load next token

    return 0;

}

//FOR_PRIRAD-> id LAVA_STRANA PRAVA_STRANA {
//FOR_PRIRAD-> {
int for_prirad(ParserData *data)
{
    int result;

    if (data->token.type == tt_id)
    {
        nodePtr id;
        if((id = check_if_id_declared(data->table_stack.table[data->table_stack.top], data->token.attribute.value)) == NULL) return ERR_SEM_BAD_DEFINITION;

        // function call cannot be on the left side here
        if(id->type == DataTypeFunc) return ERR_SYN;

        // increase return values number, in case function call comes later
        // and save id type to compare with expected return type later
        data->ret_values_num++;
        data->ret_t_arr[data->ret_values_num - 1] = id->type;

        if((result = load_token(data)) != 0) return result; // load next token
        if((result = lava_strana(data)) != 0 ) return result; // use next rule

        data->if_or_for = 4;

        if((result = prava_strana(data)) != 0 ) return result; // use next rule

        data->if_or_for = 0;
        data->ret_values_num = 0;
        data->argument_c = 0;

        if(data->token.type != tt_left_bra) return ERR_SYN;

        
        return 0;
    }else
    {
        if(data->token.type != tt_left_bra) return ERR_SYN;
        return 0;
    }
}

//RETURN_LIST-> EXPR RETURN_STATEMENT
//RETURN_LIST-> eol
int return_list(ParserData *data)
{
    int result;

    if(data->token.type == tt_eol)
    {
        nodePtr in_function = table_search(*symtable, data->in_func);
        // check if declared return list is also empty
        if(in_function->ret_cnt != 0) return ERR_SEM_BAD_ARGS;

        if((result = load_token(data)) != 0) return result; // load next token

        return 0;
    }else
    {
        data->prev_exists = 0;

        if((result = expression(data)) != 0) return result;

        data->ret_values_num++;
        data->prev_exists = 0;

        nodePtr in_function = table_search(*symtable, data->in_func);
        // check if return type is matching definition
        if(in_function->ret_types[data->ret_values_num - 1] != data->assigning_type) return ERR_SEM_BAD_ARGS;

        if((result = return_statement(data)) != 0 ) return result; // use next rule
        data->ret_values_num = 0;
        return 0;
    }
}

//RETURN_STATEMENT-> , EXPR RETURN_STATEMENT
//RETURN_STATEMENT-> eol
int return_statement(ParserData *data)
{
    int result;

    if(data->token.type == tt_eol)
    {
        if((result = load_token(data)) != 0) return result; // load next token

        return 0;
    }else if(data->token.type == tt_comma)
    {
        data->prev_exists = 0;

        if((result = load_token(data)) != 0) return result; // load next token

        if((result = expression(data)) != 0) return result;

        data->ret_values_num++;
        data->prev_exists = 0;

        nodePtr in_function = table_search(*symtable, data->in_func);
        // check if return type is matching definition
        if(in_function->ret_types[data->ret_values_num - 1] != data->assigning_type) return ERR_SEM_BAD_ARGS;

        if((result = return_statement(data)) != 0 ) return result; // use next rule

        return 0;
    }
    
    return ERR_SYN;
}

//TYPE-> int
//TYPE-> float64
//TYPE-> string
int type(ParserData *data)
{
    if ((data->token.type == tt_keyword_int)      ||
        (data->token.type == tt_keyword_float64)  ||
        (data->token.type == tt_keyword_string))
    {
        return 0;
    }
      
    return ERR_SYN;
}

