//
// Created by Tomas Cernik on 20/11/2020.
//

#ifndef IFJ20_EXPRESSION_H
#define IFJ20_EXPRESSION_H

#include "parser.h"

#define table_size 7
#define max_stack 10000

typedef enum
{
    S,    // shift <
    R,    // reduce >
    E,    // equal =
    N     // error
} TableSign;

typedef enum
{
   SymbolPlus,      // +
   SymbolMinus,     // -
   SymbolMultiply,  // *
   SymbolDivide,    // /
   SymbolLess,      // <
   SymbolMore,      // >
   SymbolLessEqual, // <=
   SymbolMoreEqual, // >=
   SymbolEqual,     // ==
   SymbolNotEqual,  // !=
   SymbolLeftPar,   // (
   SymbolRightPar,  // )
   SymbolIdentifier,// id
   SymbolInt,       // int
   SymbolDouble,    // float
   SymbolString,    // String
   SymbolDollar,    // $
   SymbolNonTerm,   //non Term
   SymbolBreak      //symbol used to break expression into pieces
} Symbol;

typedef enum
{
    RuleOperand,    // E -> i
    RuleEmptyBrackets,// E -> ()
    RuleBrackets,   // E -> (E)
    RuleLess,       // E -> E < E
    RuleMore,       // E -> E > E
    RuleLessEqual,  // E -> E <= E
    RuleMoreEqual,  // E -> E >= E
    RuleEqual,      // E -> E == E
    RuleNotEqual,   // E -> E != E
    RuleMultiply,   // E -> E * E
    RuleDivide,     // E -> E / E
    RulePlus,       // E -> E + E
    RuleMinus,       // E -> E - E
    RuleNoRule
} Rule;

typedef enum
{
    IndexPlusMinus, // +-
    IndexMulDiv,    // */
    IndexRelation,  // r - relacni operatory
    IndexLeftPar,   // (
    IndexRightPar,  // )
    IndexData,      // i (data)
    IndexDollar     // $
} TableIndex;

typedef struct stackItem{
    Symbol symbol;
    DataType dataType;
    int exist;
} stackItem;

typedef struct Stack {
    int top;
    stackItem item[max_stack];
    int numOfLeftBrackets;
    int numOfRightBrackets;
} Stack;

int expression(ParserData* data);

#endif //IFJ20_EXPRESSION_H
