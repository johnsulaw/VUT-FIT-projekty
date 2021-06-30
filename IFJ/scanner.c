//Project: IFJ 20
//File: scanner.c
//Author(s): Jan Šulavík (xsulav01)
//Date: 1. 10. 2020

#include "scanner.h"

// function decides, if character is an operator or separator
int is_operator_or_separator(char c){
    char symbols[SYMBOLS_NUMBER] = {'*', '+', '/', '-', '>', '<', '=', '!', ':', ',', ';', '(', ')', '{', '}'};

    for(int i = 0 ; i < SYMBOLS_NUMBER ; i++){
        if(c == symbols[i]){
            return 1;
        }
    }
    return 0;
}

// function returns one token from source
Token get_token(){
    Token token;
    State state;

    char prev_char = '\0';

    my_string_init(&token.attribute);
    state = STATE_START;

    // field for hex escape sequence
    char hex[] = {'\0', '\0'};
    char c;
    token.type = tt_none;

    while(1){
        c = getchar();

        /* switch loop to check and change states */
        switch (state) {
            case STATE_START:
                if(c == EOF){
                    my_strcat(&token.attribute, "EOF");
                    token.type = tt_eof;
                    return token;
                }else if(isalpha(c) || (c == '_')){
                    my_strncat(&token.attribute, c);
                    state = STATE_ID;
                }else if(isdigit(c)){
                    my_strncat(&token.attribute, c);
                    prev_char = c;
                    state = STATE_INT;
                }else if(c == '/'){
                    state = STATE_OP_DIV;
                }else if(c == '\"'){
                    state = STATE_STRING_START;
                }else if(c == ' ' || c == '\t'){
                    state = STATE_START;
                }else if(c == '*'){
                    my_strncat(&token.attribute, c);
                    token.type = tt_mul;

                    return token;
                }else if(c == '+'){
                    my_strncat(&token.attribute, c);
                    token.type = tt_add;

                    return token;
                }else if(c == '-'){
                    my_strncat(&token.attribute, c);
                    token.type = tt_sub;

                    return token;
                }else if(c == '>'){
                    my_strncat(&token.attribute, c);
                    state = STATE_OP_GT;
                }else if(c == '<'){
                    my_strncat(&token.attribute, c);
                    state = STATE_OP_LT;
                }else if(c == '='){
                    my_strncat(&token.attribute, c);
                    state = STATE_OP_ASSIGN;
                }else if(c == '!'){
                    my_strncat(&token.attribute, c);
                    state = STATE_OP_NEG;
                }else if(c == ':'){
                    my_strncat(&token.attribute, c);
                    state = STATE_COLON;
                }else if(c == ','){
                    my_strncat(&token.attribute, c);
                    token.type = tt_comma;

                    return token;
                }else if(c == ';'){
                    my_strncat(&token.attribute, c);
                    token.type = tt_semicolon;

                    return token;
                }else if(c == '('){
                    my_strncat(&token.attribute, c);
                    token.type = tt_left_par;

                    return token;
                }else if(c == ')'){
                    my_strncat(&token.attribute, c);
                    token.type = tt_right_par;

                    return token;
                }else if(c == '{'){
                    my_strncat(&token.attribute, c);
                    token.type = tt_left_bra;

                    return token;
                }else if(c == '}'){
                    my_strncat(&token.attribute, c);
                    token.type = tt_right_bra;

                    return token;
                }else if(c == '\n'){
                    token.type = tt_eol;

                    return token;
                }else{
                    token.type = tt_error;

                    return token;
                }
                break;

            case STATE_ID:
                if(c == '_' || isalpha(c) || isdigit(c)){
                    my_strncat(&token.attribute, c);
                    state = STATE_ID;
                }else if(c == ' ' || c == '\n'){  /* checking if token is a keyword/identificator*/
                    if(c == '\n'){
                        ungetc(c, stdin);
                    }
                    if(strcmp(token.attribute.value, "if") == 0){
                        token.type = tt_keyword_if;
                        return token;
                    }else if(strcmp(token.attribute.value, "else") == 0){
                        token.type = tt_keyword_else;
                        return token;
                    }else if(strcmp(token.attribute.value, "int") == 0){
                        token.type = tt_keyword_int;
                        return token;
                    }else if(strcmp(token.attribute.value, "float64") == 0){
                        token.type = tt_keyword_float64;
                        return token;
                    }else if(strcmp(token.attribute.value, "for") == 0){
                        token.type = tt_keyword_for;
                        return token;
                    }else if(strcmp(token.attribute.value, "func") == 0){
                        token.type = tt_keyword_func;
                        return token;
                    }else if(strcmp(token.attribute.value, "package") == 0){
                        token.type = tt_keyword_package;
                        return token;
                    }else if(strcmp(token.attribute.value, "return") == 0){
                        token.type = tt_keyword_return;
                        return token;
                    }else if(strcmp(token.attribute.value, "string") == 0){
                        token.type = tt_keyword_string;
                        return token;
                    }
                    //if token is not a keyword it's an identificator
                    token.type = tt_id;
                    return token;

                }else if(c == EOF){
                    ungetc(c, stdin);

                    token.type = tt_id;
                    return token;
                }else if(is_operator_or_separator(c)){
                    ungetc(c, stdin);

                    if(strcmp(token.attribute.value, "if") == 0){
                        token.type = tt_keyword_if;
                        return token;
                    }else if(strcmp(token.attribute.value, "else") == 0){
                        token.type = tt_keyword_else;
                        return token;
                    }else if(strcmp(token.attribute.value, "int") == 0){
                        token.type = tt_keyword_int;
                        return token;
                    }else if(strcmp(token.attribute.value, "float64") == 0){
                        token.type = tt_keyword_float64;
                        return token;
                    }else if(strcmp(token.attribute.value, "for") == 0){
                        token.type = tt_keyword_for;
                        return token;
                    }else if(strcmp(token.attribute.value, "func") == 0){
                        token.type = tt_keyword_func;
                        return token;
                    }else if(strcmp(token.attribute.value, "package") == 0){
                        token.type = tt_keyword_package;
                        return token;
                    }else if(strcmp(token.attribute.value, "return") == 0){
                        token.type = tt_keyword_return;
                        return token;
                    }else if(strcmp(token.attribute.value, "string") == 0){
                        token.type = tt_keyword_string;
                        return token;
                    }
                    //if token is not a keyword it's an identificator
                    token.type = tt_id;
                    return token;

                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_INT:
                if(isdigit(c) && (prev_char != '0')){
                    my_strncat(&token.attribute, c);
                    state = STATE_INT;
                }else if((c == 'e') || (c == 'E')){
                    my_strncat(&token.attribute, c);
                    state = STATE_DOUBLE_EXP;
                }else if(c == '.'){
                    my_strncat(&token.attribute, c);
                    state = STATE_DOUBLE_POINT;
                }else if(c == EOF || c == '\n'){
                    ungetc(c, stdin);

                    token.type = tt_int;
                    return token;
                }else if(isspace(c)){
                    token.type = tt_int;
                    return token;
                }else if(isalpha(c)){
                    ungetc(c, stdin);

                    token.type = tt_int;
                    return token;
                }else if(is_operator_or_separator(c)){
                    ungetc(c, stdin);

                    token.type = tt_int;
                    return token;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_DOUBLE_POINT:
                if(isdigit(c)){
                    my_strncat(&token.attribute, c);
                    state = STATE_DOUBLE_POINT_NUM;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_DOUBLE_POINT_NUM:
                if(isdigit(c)){
                    my_strncat(&token.attribute, c);
                    state = STATE_DOUBLE_POINT_NUM;
                }else if((c == 'E') || (c == 'e')){
                    my_strncat(&token.attribute, c);
                    state = STATE_DOUBLE_EXP;
                }else if(c == ' '){
                    token.type = tt_double;
                    return token;
                }else if(c == EOF || c == '\n'){
                    ungetc(c, stdin);

                    token.type = tt_double;
                    return token;
                }else if(is_operator_or_separator(c)) {
                    ungetc(c, stdin);

                    token.type = tt_double;
                    return token;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_DOUBLE_EXP:
                if(isdigit(c)){
                    my_strncat(&token.attribute, c);
                    state = STATE_DOUBLE_EXP_NUM;
                }else if((c == '-') || (c == '+')){
                    my_strncat(&token.attribute, c);
                    state = STATE_DOUBLE_EXP_SIGN;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_DOUBLE_EXP_SIGN:
                if(isdigit(c)){
                    my_strncat(&token.attribute, c);
                    state = STATE_DOUBLE_EXP_NUM;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_DOUBLE_EXP_NUM:
                if(isdigit(c)){
                    my_strncat(&token.attribute, c);
                    state = STATE_DOUBLE_EXP_NUM;
                }else if(c == ' '){
                    token.type = tt_double;
                    return token;
                }else if(c == EOF || c == '\n') {
                    ungetc(c, stdin);

                    token.type = tt_double;
                    return token;
                }else if(is_operator_or_separator(c)) {
                    ungetc(c, stdin);

                    token.type = tt_double;
                    return token;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_STRING_START:
                if(c == '\"'){
                    //if string is empty, remove the first "
                    if(token.attribute.length == 1){
                        token.attribute.value[0] = '\0';
                    }
                    state = STATE_STRING_END;
                }else if(c == '\\'){
                    state = STATE_STRING_ESCAPE;
                }else if(c > 31){
                    my_strncat(&token.attribute, c);
                    state = STATE_STRING_CHARACTER;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_STRING_CHARACTER:
                if(c == '\\'){
                    state = STATE_STRING_ESCAPE;
                }else if(c > 31 && c != '\"'){
                    my_strncat(&token.attribute, c);
                    state = STATE_STRING_CHARACTER;
                }else if(c == '\"'){
                    state = STATE_STRING_END;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_STRING_ESCAPE:
                if(c == '\"'){
                    my_strncat(&token.attribute, '\"');
                    state = STATE_STRING_CHARACTER;
                }else if(c == '\\'){
                    my_strncat(&token.attribute, '\\');
                    state = STATE_STRING_CHARACTER;
                }else if(c == 't'){
                    my_strncat(&token.attribute, '\t');
                    state = STATE_STRING_CHARACTER;
                }else if(c == 'n'){
                    my_strncat(&token.attribute, '\n');
                    state = STATE_STRING_CHARACTER;
                }else if(c == 'x'){
                    state = STATE_STRING_HEX1;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_STRING_HEX1:
                if(isdigit(c) || ((c >= 'A') && (c <= 'f'))){
                    hex[0] = c;
                    state = STATE_STRING_HEX2;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_STRING_HEX2:
                if(isdigit(c) || ((c >= 'A') && (c <= 'f'))){
                    hex[1] = c;
                    char x = (char)strtol(hex, NULL, 16);

                    my_strncat(&token.attribute, x);
                    state = STATE_STRING_CHARACTER;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

            case STATE_STRING_END:
                if(!isspace(c) || c == '\n'){
                    ungetc(c, stdin);

                    token.type = tt_string;
                    return token;
                }else if(c == EOF || c == ')'){
                    ungetc(c, stdin);

                    token.type = tt_string;
                    return token;
                }else if(is_operator_or_separator(c)){
                    ungetc(c, stdin);

                    token.type = tt_string;
                    return token;
                }else if(c == ' '){
                    token.type = tt_string;
                    return token;
                }else{
                    token.type = tt_error;
                    return token;
                }
                break;

                /* comments are obsolete, just go through them */
            case STATE_COMMENT_LINE:
                if(c == '\n'){
                    token.type = tt_eol;
                    return token;
                }else if(c == EOF){
                    ungetc(c, stdin);

                    state = STATE_START;
                }else{
                    state = STATE_COMMENT_LINE;
                }
                break;

            case STATE_COMMENT_MULTILINE:
                if(c == '*'){
                    state = STATE_COMMENT_MULTILINE_END;
                }else if(c == EOF){
                    token.type = tt_error;
                    return token;
                }else{
                    state = STATE_COMMENT_MULTILINE;
                }
                break;

            case STATE_COMMENT_MULTILINE_END:
                if(c == '/'){
                    state = STATE_START;
                }else{
                    state = STATE_COMMENT_MULTILINE;
                }
                break;

            case STATE_OP_DIV:
                if(c == '/'){
                    state = STATE_COMMENT_LINE;
                    break;
                }else if(c == '*'){
                    state = STATE_COMMENT_MULTILINE;
                    break;
                }else{
                    if(!isspace(c)){
                        ungetc(c, stdin);
                    }else if(c == '\n'){
                        ungetc(c, stdin);
                    }
                    my_strncat(&token.attribute, '/');
                    token.type = tt_div;
                    return token;
                }

                break;

            case STATE_OP_GT:
                if(c == '='){
                    my_strncat(&token.attribute, c);
                    state = STATE_OP_EQGT;
                    break;
                }else if(!isspace(c) || c == '\n'){
                    ungetc(c, stdin);
                }
                token.type = tt_gt;
                return token;
                break;

            case STATE_OP_LT:
                if(c == '='){
                    my_strncat(&token.attribute, c);
                    state = STATE_OP_EQLT;
                    break;
                }else if(!isspace(c) || c == '\n') {
                    ungetc(c, stdin);
                }
                token.type = tt_lt;
                return token;
                break;

            case STATE_OP_ASSIGN:
                if(c == '='){
                    my_strncat(&token.attribute, c);
                    state = STATE_OP_EQ;
                    break;
                }else if(!isspace(c) || c == '\n'){
                    ungetc(c, stdin);
                }

                token.type = tt_assign;
                return token;

            case STATE_OP_NEG:
                if(c == '='){
                    my_strncat(&token.attribute, c);
                    state = STATE_OP_NEQ;
                    break;
                }else if(!isspace(c) || c == '\n'){
                    ungetc(c, stdin);
                }

                token.type = tt_error;
                return token;


            case STATE_COLON:
                if(c == '='){
                    my_strncat(&token.attribute, c);
                    state = STATE_OP_DEFINE;
                    break;
                }else if(!isspace(c) || c == '\n'){
                    ungetc(c, stdin);
                }

                token.type = tt_error;
                return token;


            case STATE_OP_DEFINE:
                token.type = tt_define;
                if(!isspace(c) || c == '\n'){
                    ungetc(c, stdin);
                }

                return token;

            case STATE_OP_EQLT:
                token.type = tt_eqlt;
                if(!isspace(c) || c == '\n'){
                    ungetc(c, stdin);
                }

                return token;

            case STATE_OP_EQGT:
                token.type = tt_eqgt;
                if(!isspace(c) || c == '\n'){
                    ungetc(c, stdin);
                }

                return token;

            case STATE_OP_EQ:
                token.type = tt_eq;
                if(!isspace(c) || c == '\n'){
                    ungetc(c, stdin);
                }

                return token;

            case STATE_OP_NEQ:
                token.type = tt_neq;
                if(!isspace(c) || c == '\n'){
                    ungetc(c, stdin);
                }

                return token;
        }
    }
}
