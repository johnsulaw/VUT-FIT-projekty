///Project: IFJ 20
///File: code_gen.h
///Author(s): Matěj Zedník (xzedni15)
#ifndef IFJ20_CODE_GEN_H
#define IFJ20_CODE_GEN_H
#include "expression.h"
#include "parser.h"
#include "scanner.h"
#include "dynamic_string.h"

/*MyString functions*/
void str_Init();
void tmpFile_addRow(char* content);
void tmpFile_ClearAll();
void tmpFile_PrintF();

/*Main functions for header, begin and end of code*/
void genCode_Header();
void genCode_MainStart();
void genCode_MainEnd();

/*Variables definitions*/
void genCode_DefVar_Global(char* name);
void genCode_DefVar_Local(char* name);
void genCode_DefVar_Temp(char* name);


/*Function*/
void genCode_FunctionStart(char* name);
char* genCode_FunctionRetVal(DataType type);
void genCode_FunctionCall(char* name);
void genCode_FunctionParamVal(char* name, DataType type);
void genCode_FunctionEnd(char* name);

/*PreFunction*/
void genCode_INPUTS();
void genCode_INPUTI();
void genCode_INPUTF();
void genCode_PRINT();
void genCode_LEN();
void genCode_SUBSTR();
void genCode_ORD();
void genCode_CHR();

/*Push and Pop functions*/
void genCode_Push_Local(char* name);
void genCode_Push_Global(char* name);
void genCode_Push_Temp(char* name);
void genCode_PushVar(char* name, Token token);
void genCode_POPLOCAL(char *name);//DO proměnné LF@name dá hodnotu ze zásobníku
void genCode_POPGLOBAL(char *name);//DO proměnné GF@name dá hodnotu ze zásobníku
void genCode_POPTEMP(char *name);
void genCode_PushInt(char* a);
void genCode_PushFloat(char* a);
void genCode_PushBool(char* a);
void genCode_PushString(char* a);

/*String functions*/
void genCode_CONCATS();
void genCode_STRLENS();
void genCode_GETCHARS();//first push string, second push index
void genCode_SETCHARS();//first push index, second push string

/*Mathematics and comparing functions*/
void genCode_ADD();
void genCode_SUB();
void genCode_MUL();
void genCode_DIV();
void genCode_IDIV();
void genCode_Compare(Rule r);

/*Condition functions(IF)*/
void genCode_if_MainStart(Rule r);
void generate_if_else();
void genCode_if_EndLabel();

/*Cycle functions(FOR)*/
/*First call ForStart*/
/*Second if 3.part is not epmty declare a increase*/
/*Third call ForIncEnd and then declare a body of for*/
/*And if For is complete inside call ForEnd */
void genCode_ForStart();
void genCode_ForIncStart(); //call after generating start of for
void genCode_ForIncEnd();
void genCode_ForEnd();

/*Frames*/
void genCode_CreateFrame();
void genCode_PushFrame();
void genCode_PopFrame();

#endif //IFJ20_CODE_GEN_H