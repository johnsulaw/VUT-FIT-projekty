//
// Created by Tomas Cernik on 20/11/2020.
//

#include "scanner.h"
#include "expression.h"
#include "parser.h"
#include "error.h"
#include "dynamic_string.h"
#include "symtable.h"
#include "code_gen.h"
#include <stdbool.h>
#include <stdlib.h>

//zjednodusena precedencni tabulka
int precedential_table[table_size][table_size] = {
     // +-  */ r  (  )  i  $
        {R, S, R, S, R, S, R}, // +-
        {R, R, R, S, R, S, R}, // */
        {S, S, N, S, R, S, R}, // r (relational operators)
        {S, S, S, S, E, S, N}, // (
        {R, R, R, N, R, N, R}, // )
        {R, R, R, N, R, N, R}, // i (data)
        {S, S, S, S, N, S, N}, // $
};

Symbol get_symbol(TokenType typeOfToken) {
    switch (typeOfToken) {
        case tt_add:
            return SymbolPlus;
        case tt_sub:
            return SymbolMinus;
        case tt_mul:
            return SymbolMultiply;
        case tt_div:
            return SymbolDivide;
        case tt_lt:
            return SymbolLess;
        case tt_gt:
            return SymbolMore;
        case tt_eqlt:
            return SymbolLessEqual;
        case tt_eqgt:
            return SymbolMoreEqual;
        case tt_eq:
            return SymbolEqual;
        case tt_neq:
            return SymbolNotEqual;
        case tt_left_par:
            return SymbolLeftPar;
        case tt_right_par:
            return SymbolRightPar;
        case tt_id:
            return SymbolIdentifier;
        case tt_int:
            return SymbolInt;
        case tt_double:
            return SymbolDouble;
        case tt_string:
            return SymbolString;
        default:
            return SymbolDollar;
    }
}

TableIndex get_index(Symbol symbolToIndex){
    switch (symbolToIndex) {
        case SymbolPlus:
        case SymbolMinus:
            return IndexPlusMinus;
        case SymbolMultiply:
        case SymbolDivide:
            return IndexMulDiv;
        case SymbolLess:
        case SymbolMore:
        case SymbolLessEqual:
        case SymbolMoreEqual:
        case SymbolEqual:
        case SymbolNotEqual:
            return IndexRelation;
        case SymbolLeftPar:
            return IndexLeftPar;
        case SymbolRightPar:
            return IndexRightPar;
        case SymbolIdentifier:
        case SymbolInt:
        case SymbolDouble:
        case SymbolString:
            return IndexData;
        default:
            return IndexDollar;
    }
}


//-----------------funkce stacku------------------


Stack createStack()
{
    Stack stack;
    stack.top = 0;
    stack.item[0].symbol = SymbolDollar;
    stack.item[0].exist = true;
    stack.item[0].dataType = DataTypeUndefined;

    for (int i = 1; i < max_stack; i++){
        stack.item[i].exist = false;
    }
    return stack;
}

bool is_full (Stack *stack) {
    return stack->top == max_stack-1;
}

bool is_empty (Stack *stack){
    return stack->top == -1;
}

int stack_push (Stack *stack, Symbol symbol, DataType typDat){
    if(typDat == DataTypeUnderscore)
        return ERR_SEM_OTHERS;
    if (is_full(stack))
        return ERR_INTERNAL;
    ++stack->top;
    stack->item[stack->top].symbol = symbol;
    stack->item[stack->top].dataType = typDat;
    stack->item[stack->top].exist = true;
    return 0;
}

bool stack_pop (Stack *stack, int popInt){
    for (;popInt>0;popInt--){
        if (is_empty(stack))
            return false;
        stack->item[stack->top].exist = false;
        stack->item[stack->top].dataType = DataTypeUndefined;
        stack->top--;
    }
    return true;
}


//--------------------obsluzne funkce pro zpracovani vyrazu----------------


//spocita pocet symbolu pred Symbolem Break kde konci cast kterou redukujem
int count_sym_to_reduce(Stack *symStack, int *found){
    int i = symStack->top;
    int count = 0;
    while (i > 0) {
        if(symStack->item[i].symbol == SymbolBreak){
            *found = true;
            return count;
        }
        else{
            count++;
            *found = false;
        }
        i--;
    }
    return count;
}

DataType get_data_type(Token *token, ParserData* pData){
    nodePtr point;
    switch (token->type) {
        case tt_int:
            return DataTypeInt;
        case tt_double:
            return DataTypeDouble;
        case tt_string:
            return DataTypeString;
        case tt_id:
            point = table_search(*pData->table_stack.table[pData->table_stack.top], token->attribute.value);
            return point->type;
        default:
            return DataTypeUndefined;
    }
}

Symbol get_top_terminal(Stack *symStack){
    int i = symStack->top;
    for (;i >= 0; i--){
        if (symStack->item[i].symbol != SymbolBreak && symStack->item[i].symbol != SymbolNonTerm){
            return symStack->item[i].symbol;
        }
    }
}

bool stack_insert_break(Stack *symStack){
    stackItem pom;
    stackItem pom2;
    int toMove;
    int topPlusOne = symStack->top+1;
    int i = symStack->top;
    for (;i >= 0; i--){
        if (symStack->item[i].symbol != SymbolBreak && symStack->item[i].symbol != SymbolNonTerm){
            if (i+1 < max_stack) {
                pom = symStack->item[i + 1];
                toMove = symStack->top - i;

                symStack->item[i + 1].symbol = SymbolBreak;
                symStack->item[i + 1].dataType = DataTypeUndefined;
            }
            else {
                return false;
            }
            if (topPlusOne < max_stack) {
                for(;toMove > 0; toMove--){
                    pom2 = symStack->item[topPlusOne-1];
                    symStack->item[topPlusOne] = pom2;
                    topPlusOne--;
                }
                symStack->item[i+2] = pom;
            }
            else {
                return false;
            }
            symStack->top++;
            return true;
        }
    }
}


//kontrola semantiky
int check_semantics(Rule reduceRule, stackItem first, stackItem second, stackItem third, DataType* resultType) {

    //kontrola typu prevadeneho operandu
    if(reduceRule == RuleOperand){
        if (first.dataType == DataTypeUndefined) return ERR_SEM_BAD_DEFINITION;
        if (first.dataType == DataTypeBool) return ERR_SEM_TYPE_COMP;
    }

    //item mezi zavorkami musi byt definovany
    if(reduceRule == RuleBrackets && second.dataType == DataTypeUndefined) return ERR_SEM_BAD_DEFINITION;



    //pokud nejde o pravidlo operandu, itemu mezi zavorkami ani prazdnych zavorek tak first a third item musi byt definovany  a nesmi byt bool
    if(reduceRule != RuleOperand && reduceRule != RuleBrackets && reduceRule != RuleEmptyBrackets) {
        if (first.dataType == DataTypeUndefined || third.dataType == DataTypeUndefined) return ERR_SEM_BAD_DEFINITION;
        if (first.dataType == DataTypeBool || third.dataType == DataTypeBool) return ERR_SEM_TYPE_COMP;
    }

    switch (reduceRule){
        case RuleOperand:
            *resultType = first.dataType;
            break;
        case RuleEmptyBrackets:
            return ERR_SYN;
        case RuleBrackets:
            *resultType = second.dataType;
            break;
        case RuleLess:
        case RuleMore:
        case RuleLessEqual:
        case RuleMoreEqual:
        case RuleEqual:
        case RuleNotEqual:
            if (first.dataType != third.dataType) return ERR_SEM_TYPE_COMP;
            *resultType = DataTypeBool;
            break;
        case RuleMultiply:
            if(first.dataType == DataTypeInt && third.dataType == DataTypeInt){
                *resultType = DataTypeInt;
                break;
            }
            else if(first.dataType == DataTypeDouble && third.dataType == DataTypeDouble){
                *resultType = DataTypeDouble;
                break;
            }
            else {
                return ERR_SEM_TYPE_COMP;
            }
        case RuleDivide:
            if(first.dataType == DataTypeInt && third.dataType == DataTypeInt){
                *resultType = DataTypeInt;
                break;
            }
            else if(first.dataType == DataTypeDouble && third.dataType == DataTypeDouble){
                *resultType = DataTypeDouble;
                break;
            }
            else {
                return ERR_SEM_TYPE_COMP;
            }
        case RulePlus:
            if(first.dataType == DataTypeInt && third.dataType == DataTypeInt){
                *resultType = DataTypeInt;
                break;
            }
            else if(first.dataType == DataTypeDouble && third.dataType == DataTypeDouble){
                *resultType = DataTypeDouble;
                break;
            }
            else if(first.dataType == DataTypeString && third.dataType == DataTypeString){
                *resultType = DataTypeString;
                break;
            }
            else {
                return ERR_SEM_TYPE_COMP;
            }
        case RuleMinus:
            if(first.dataType == DataTypeInt && third.dataType == DataTypeInt){
                *resultType = DataTypeInt;
                break;
            }
            else if(first.dataType == DataTypeDouble && third.dataType == DataTypeDouble){
                *resultType = DataTypeDouble;
                break;
            }
            else {
                return ERR_SEM_TYPE_COMP;
            }
        default:
            break;
    }
    return 0;
}

//zjisti pravidlo pro redukci a vrati ho
Rule test_reduce_rule(stackItem first, stackItem second, stackItem third, int count){
    switch (count){
        case 1:
            if (first.symbol == SymbolInt || first.symbol == SymbolDouble || first.symbol == SymbolString || first.symbol == SymbolIdentifier){
                return RuleOperand;
            }
            else {
                return RuleNoRule;
            }
        case 2:
            if (first.symbol == SymbolLeftPar && second.symbol == SymbolRightPar){
                return RuleEmptyBrackets;
            }
            else {
                return RuleNoRule;
            }
        case 3:
            if ((first.symbol == SymbolLeftPar)&&(third.symbol == SymbolRightPar)&&(second.symbol == SymbolNonTerm)){
                return RuleBrackets;
            }
            else if(first.symbol == SymbolNonTerm && third.symbol == SymbolNonTerm) {
                switch (second.symbol) {
                    case SymbolPlus:
                        return RulePlus;
                    case SymbolMinus:
                        return RuleMinus;
                    case SymbolMultiply:
                        return RuleMultiply;
                    case SymbolDivide:
                        return RuleDivide;
                    case SymbolLess:
                        return RuleLess;
                    case SymbolMore:
                        return RuleMore;
                    case SymbolLessEqual:
                        return RuleLessEqual;
                    case SymbolMoreEqual:
                        return RuleMoreEqual;
                    case SymbolEqual:
                        return RuleEqual;
                    case SymbolNotEqual:
                        return RuleNotEqual;
                    default:
                        return RuleNoRule;
                }
            }
            else {
                return RuleNoRule;
            }
    }
}

//redukuje popne redukovane symboly ze stacku a pushne vysledek redukce
int reduce(Stack *symStack, Token newToken){
    int result;
    int found;
    DataType resultType;
    Rule reduceRule;
    int count = count_sym_to_reduce(symStack, &found);

    stackItem first;
    stackItem second;
    stackItem third;

    //pokud nebyl nalezen symbol break - Syntakticka chyba
    if (found == 0){
        return ERR_SYN;
    }

    switch (count){
        case 1: // 1 symbol na redukci
            first = symStack->item[symStack->top];
            first.exist = true;

            reduceRule = test_reduce_rule(first, second, third, count);
            break;
        case 2: // 2 symboli na redukci
            first = symStack->item[symStack->top-1];
            first.exist = true;
            second = symStack->item[symStack->top];
            second.exist = true;

            reduceRule = test_reduce_rule(first, second, third, count);
            break;
        case 3: // 3 symboli k redukci
            first = symStack->item[symStack->top-2];
            first.exist = true;
            second = symStack->item[symStack->top-1];
            second.exist = true;
            third = symStack->item[symStack->top];
            third.exist = true;

            reduceRule = test_reduce_rule(first, second, third, count);
            break;
        default:
            reduceRule = RuleNoRule;
            break;
    }

    if (reduceRule != RuleNoRule){
        if ((result = check_semantics(reduceRule, first, second, third, &resultType)) != 0){
            return result;
        }

        int i = 2;
        int zero = 0;
        //volani funkci code genu pro jednotlive operace
        switch(reduceRule){
            case RulePlus:
                if(first.dataType == DataTypeString){
                    genCode_CONCATS();
                }else{
                    genCode_ADD();
                }
                break;
            case RuleMinus:
                genCode_SUB();
                break;
            case RuleMultiply:
                genCode_MUL();
                break;
            case RuleDivide:
                if(first.dataType == DataTypeDouble){
                    if(newToken.attribute.value[0] == '0' && newToken.attribute.value[1] == '.'){
                        for(;newToken.attribute.value[i] != '\0';i++){
                            if(newToken.attribute.value[i] == '0'){
                                zero = 1;
                            }else{
                                zero = 0;
                            }
                        }
                    }
                    if(zero == 1){
                        return ERR_SEM_ZERO_DIVISION;
                    }
                    genCode_IDIV();
                }else{
                    if(newToken.attribute.value[0] == '0'){
                        return ERR_SEM_ZERO_DIVISION;
                    }
                    genCode_DIV();
                }
                break;
            case RuleLess:
            case RuleMore:
            case RuleLessEqual:
            case RuleMoreEqual:
            case RuleEqual:
            case RuleNotEqual:
                genCode_Compare(reduceRule);
                break;
            default:
                break;
        }

        stack_pop(symStack, count + 1);
        stack_push(symStack, SymbolNonTerm, resultType);

        return 0;
    }
    else{
        return ERR_SYN;
    }
}


//-------------------Main funkce zpracovani vyrazu-----------------------


int expression(ParserData *data){
    Stack symStack = createStack();
    symStack.numOfLeftBrackets = 0;
    symStack.numOfRightBrackets = 0;
    Symbol topSymbol;
    Symbol actualSymbol;
    int gotNewToken = true;
    int result;
    int end = false;
    int next = false;
    do {
        topSymbol = get_top_terminal(&symStack);
        if(data->prev_exists == 1){
            actualSymbol = get_symbol(data->prev_token.type);
        }else{
            actualSymbol = get_symbol(data->token.type);
        }


        if (gotNewToken == 1){
            if(actualSymbol == SymbolLeftPar){
                symStack.numOfLeftBrackets++;
            }
            if(actualSymbol == SymbolRightPar){
                symStack.numOfRightBrackets++;

                if(symStack.numOfRightBrackets > symStack.numOfLeftBrackets && data->expr_function == 1){
                    TokenType pomTokenType;
                    //dalsi token
                    if(data->prev_exists == 1){
                        // if previous token exists do not load new token, it is already loaded
                        data->prev_exists = 0;
                    }else{
                        data->token = get_token();
                    }
                    if(data->prev_exists == 1){
                        pomTokenType = data->prev_token.type;
                    }else{
                        pomTokenType = data->token.type;
                    }
                    if(pomTokenType == tt_eol){
                        actualSymbol = SymbolDollar;
                    }
                    else{
                        return ERR_SYN;
                    }
                }

                if(symStack.numOfRightBrackets > symStack.numOfLeftBrackets && data->expr_function == 0){
                    return ERR_SYN;
                }
            }
        }


        switch(precedential_table[get_index(topSymbol)][get_index(actualSymbol)]){
            Token newToken;
            case S:
                if(!stack_insert_break(&symStack)){
                    return ERR_INTERNAL;
                }
                if(data->prev_exists == 1){
                    if(data->prev_token.type == tt_id){
                        if(check_if_id_declared(data->table_stack.table[data->table_stack.top], data->prev_token.attribute.value) == NULL) return ERR_SEM_BAD_DEFINITION;
                    }
                    if((result = stack_push(&symStack, actualSymbol, get_data_type(&data->prev_token, data))) != 0){
                        return result;
                    }
                    newToken = data->prev_token;
                }else{
                    if(data->token.type == tt_id){
                        if(check_if_id_declared(data->table_stack.table[data->table_stack.top], data->token.attribute.value) == NULL) return ERR_SEM_BAD_DEFINITION;
                    }
                    if((result = stack_push(&symStack, actualSymbol, get_data_type(&data->token, data))) != 0){
                        return result;
                    }
                    newToken = data->token;
                }

                // zavola funkce code genu podle typu tokenu - pokud jde o nejaka data
                switch (newToken.type) {
                    case tt_id:
                        genCode_Push_Local(newToken.attribute.value);
                        break;
                    case tt_string:
                        genCode_PushString(newToken.attribute.value);
                        break;
                    case tt_double:
                        genCode_PushFloat(newToken.attribute.value);
                        break;
                    case tt_int:
                        genCode_PushInt(newToken.attribute.value);
                        break;
                    default:
                        break;
                }

                next = true;
                break;
            case R:
                if((result = reduce(&symStack, newToken)) != 0){
                    return result;
                }
                next = false;
                break;
            case E:
                if(data->prev_exists == 1){
                    if(data->prev_token.type == tt_id){
                        if(check_if_id_declared(data->table_stack.table[data->table_stack.top], data->token.attribute.value) == NULL) return ERR_SEM_BAD_DEFINITION;
                    }
                    if((result = stack_push(&symStack, actualSymbol, get_data_type(&data->prev_token, data))) != 0){
                        return result;
                    }
                }else{
                    if(data->token.type == tt_id){
                        if(check_if_id_declared(data->table_stack.table[data->table_stack.top], data->token.attribute.value) == NULL) return ERR_SEM_BAD_DEFINITION;
                    }
                    if((result = stack_push(&symStack, actualSymbol, get_data_type(&data->token, data))) != 0){
                        return result;
                    }
                }
                next = true;
                break;
            case N:
                if (topSymbol == SymbolDollar && actualSymbol == SymbolDollar){
                    end = true;
                }
                else {
                    return ERR_SYN;
                }
                next = false;
                break;
            default:
                return ERR_INTERNAL;
        }
        if (next == true){
            gotNewToken = true;
            if(data->prev_exists == 1){
                // if previous token exists do not load new token, it is already loaded
                data->prev_exists = 0;
            }else{
                data->token = get_token();
            }
        }
        else{
            gotNewToken = false;
        }
    } while(end != 1);


    if(data->if_or_for == 1 || data->if_or_for == 3){
        if(symStack.item[symStack.top].dataType != DataTypeBool){
            return ERR_SEM_TYPE_COMP;
        }
    }

    data->assigning_type = symStack.item[symStack.top].dataType;
    data->expr_function = 0;

    return 0;
}