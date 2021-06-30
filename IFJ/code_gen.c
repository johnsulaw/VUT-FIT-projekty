//Project: IFJ 20
//File: code_gen.c
//Author(s): Matej Zednik (xzedni15)
//Date: 1. 12. 2020


#include <stdio.h>
#include <stdlib.h>
#include "code_gen.h"


/*-----Definition Temporary File-----*/
/*tmpFile is for temporarily save of Code*/
MyString* str;
char* inc_for = "";
bool for_inc = false;
int str_len = 0;
bool this_is_for = false;
int idfor = -1;
char*ids;
char*fid;
int idif = -1;
int idfuc = 0;

void str_Init() {
	my_string_init(str);
}

 /*Adding row of code to temporary file*/
void tmpFile_addRow(char* content) {
	if (!for_inc) {
		my_strcat(str, content);
		str_len = str_len + (unsigned)strlen(content);
	}
	else {
		strcat(inc_for, content);
	}
}

/*Clear temporary file*/
void tmpFile_ClearAll() {
	my_strclear(str);
	my_string_free(str);
}

/*Print all row of temporary file*/
void tmpFile_PrintF() {
	for (int i = 0; i < str_len; i++) {
		printf("%s", str->value[i]);
	}
}
/*-----End of definition of Temporary File-----*/


/*-----FUNCTION-----*/
/*Generates starting code for function*/
void genCode_FunctionStart(char* name) {
	tmpFile_addRow("LABEL $");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
	tmpFile_addRow("PUSHFRAME \n");
}

/*Generates definitions of variable in global*/
void genCode_DefVar_Global(char* name) {
	tmpFile_addRow("DEFVAR GF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}

/*Generates definitions of variable in local*/
void genCode_DefVar_Local(char* name) {
	tmpFile_addRow("DEFVAR LF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}

/*Generates definitions of variable in temp*/
void genCode_DefVar_Temp(char* name) {
	tmpFile_addRow("DEFVAR TF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}



/*Generates code for read value to Global stack*/
void genCode_Read_Global(char* name,DataType type) {
	genCode_DefVar_Global(name);
	tmpFile_addRow("READ GF@");
	tmpFile_addRow(name);
	switch (type) {
	case DataTypeInt: {
		tmpFile_addRow(" int");
		break;
	}
	case DataTypeDouble: {
		tmpFile_addRow(" float");
		break;
	}
	case DataTypeString: {
		tmpFile_addRow(" string");
		break;
	}
	case DataTypeBool: {
		tmpFile_addRow(" bool");
		break;
	}
	}
	tmpFile_addRow("\n");
}

/*Generates code for write value*/
void genCode_Write_Global(char* name) {
	tmpFile_addRow("WRITE GF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}


/*Generates code for create return value of function*/
char* genCode_FunctionRetVal(DataType type) {
	tmpFile_addRow("DEFVAR LF@%retval");
	char*retval_id = "";
	sprintf(retval_id, "%d", idfuc);
	idfuc++;
	tmpFile_addRow(retval_id);
	tmpFile_addRow("\n");
	tmpFile_addRow("MOVE LF@%retval");
	tmpFile_addRow(retval_id);
	tmpFile_addRow(" ");
	switch (type) {
		case DataTypeInt: {
			tmpFile_addRow("int@0"); 
			break; 
		}
		case DataTypeDouble: {
			tmpFile_addRow("float@0.0");
			break; 
		}
		case DataTypeString: {
			tmpFile_addRow("string@"); 
			break; 
		}
		case DataTypeBool: {
			tmpFile_addRow("bool@false"); 
			break; 
		}
	}
	tmpFile_addRow("\n");
	return retval_id;
}

/*Generates code for calling function*/
void genCode_FunctionCall(char* name) {
	tmpFile_addRow("CALL $");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}

/*Generates code for create parameter value of function*/
void genCode_FunctionParamVal(char* name, DataType type) {
		genCode_DefVar_Local(name);
		tmpFile_addRow("MOVE LF@");
		tmpFile_addRow(name);
		switch (type) {
		case DataTypeInt: {
			tmpFile_addRow(" int@0");
			break;
		}
		case DataTypeDouble: {
			tmpFile_addRow(" float@0.0");
			break;
		}
		case DataTypeString: {
			tmpFile_addRow(" string@");
			break;
		}
		case DataTypeBool: {
			tmpFile_addRow(" bool@false");
			break;
		}
		}
}

/*Generates ending code for function*/
void genCode_FunctionEnd(char* name) {
	tmpFile_addRow("LABEL $");
	tmpFile_addRow(name);
	tmpFile_addRow(" %return\n");
	tmpFile_addRow("POPFRAME\n");
	tmpFile_addRow("RETURN\n");
}

/*Generates code for create a frame*/
void genCode_CreateFrame() {
	tmpFile_addRow("CREATEFRAME\n");
}

/*Generates code for push a frame*/
void genCode_PushFrame() {
	tmpFile_addRow("PUSHFRAME\n");
}

/*Generates code for pop a frame*/
void genCode_PopFrame() {
	tmpFile_addRow("POPFRAME\n");
}

/*generates code for assign variables*/
void genCode_POPLOCAL(char *name) {
	tmpFile_addRow("POPS LF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}

/*generates code for assign variables*/
void genCode_POPGLOBAL(char *name) {
	tmpFile_addRow("POPS GF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}

/*generates code for assign variables*/
void genCode_POPTEMP(char *name) {
	tmpFile_addRow("POPS TF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}

/*generates code for push symbol of variables to local stack*/
void genCode_Push_Local(char* name) {
	tmpFile_addRow("PUSHS LF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}

/*generates code for push symbol of variables to global stack*/
void genCode_Push_Global(char* name) {
	tmpFile_addRow("PUSHS GF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}

/*generates code for push symbol of variables to global stack*/
void genCode_Push_Temp(char* name) {
	tmpFile_addRow("PUSHS TF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}
/*-----END OF FUNCTION-----*/


/*-----PREFUNCTION-----*/

/*generates code for IFJ20 functions inputs*/
void genCode_INPUTS() {
	tmpFile_addRow("DEFVAR LF@%retval\n");
	tmpFile_addRow("MOVE LF@%retval nil@nil\n");
	tmpFile_addRow("READ LF@%retval string\n");
	genCode_PopFrame();
	tmpFile_addRow("RETURN\n");
}

/*generates code for IFJ20 functions inputi*/
void genCode_INPUTI() {
	tmpFile_addRow("DEFVAR LF@%retval\n");
	tmpFile_addRow("MOVE LF@%retval nil@nil\n");
	tmpFile_addRow("READ LF@%retval int\n");
	genCode_PopFrame();
	tmpFile_addRow("RETURN\n");
}

/*generates code for IFJ20 functions inputf*/
void genCode_INPUTF() {
	tmpFile_addRow("DEFVAR LF@%retval\n");
	tmpFile_addRow("MOVE LF@%retval nil@nil\n");
	tmpFile_addRow("READ LF@%retval float\n");
	genCode_PopFrame();
	tmpFile_addRow("RETURN\n");
}

/*generates code for IFJ20 functions print*/
void genCode_PRINT() {
	tmpFile_addRow("DEFVAR LF@%retval\n");
	tmpFile_addRow("MOVE LF@%retval nil@nil\n");
	tmpFile_addRow("DEFVAR LF@%TOWRITE\n");
	tmpFile_addRow("LABEL $%print_while_start\n"); 
	tmpFile_addRow("JUMPIFEQ $%print_while_end LF@%0 int@0\n");
	tmpFile_addRow("POPS LF@%TOWRITE\n");
	tmpFile_addRow("WRITE LF@%TOWRITE\n");
	tmpFile_addRow("SUB LF@%0 LF@%0 int@1\n");
	tmpFile_addRow("JUMPIFEQ $%print_while_end LF@%0 int@0\n");
	tmpFile_addRow("INT2CHAR LF@%TOWRITE int@32\n");
	tmpFile_addRow("WRITE LF@%TOWRITE\n");
	tmpFile_addRow("JUMP $%print_while_start\n");
	tmpFile_addRow("LABEL $%print_while_end\n");
	tmpFile_addRow("INT2CHAR LF@%TOWRITE int@10\n");
	tmpFile_addRow("WRITE LF@%TOWRITE\n");
	genCode_PopFrame();
	tmpFile_addRow("RETURN\n");
}

/*generates code for IFJ20 functions len*/
void genCode_LEN() {
	tmpFile_addRow("DEFVAR LF@%retval\n");
	tmpFile_addRow("STRLEN LF@%retval LF@%0\n");
	genCode_PopFrame();
	tmpFile_addRow("RETURN\n");
}

/*generates code for IFJ20 functions substr*/
void genCode_SUBSTR() {
	tmpFile_addRow("DEFVAR LF@%retval\n");
	tmpFile_addRow("MOVE LF@%retval nil@nil\n");
	tmpFile_addRow("DEFVAR LF@%substr_res\n");
	tmpFile_addRow("DEFVAR LF@%I_type\n");
	tmpFile_addRow("DEFVAR LF@%N_type\n");
	tmpFile_addRow("DEFVAR LF@%str_len\n");
	tmpFile_addRow("STRLEN LF@%str_len LF@%0\n");
	tmpFile_addRow("TYPE LF@%I_type LF@%1\n");
	tmpFile_addRow("TYPE LF@%N_type LF@%2\n");
	tmpFile_addRow("JUMP $%substr_conv_end\n");
	tmpFile_addRow("LABEL $%Itoint\n");
	tmpFile_addRow("FLOAT2INT LF@%1 LF@%1\n");
	tmpFile_addRow("JUMP $%substr_conv_end\n");
	tmpFile_addRow("LABEL $%Ntoint\n");
	tmpFile_addRow("FLOAT2INT LF@%2 LF@%2\n");
	tmpFile_addRow("JUMP $%substr_type_end\n");
	tmpFile_addRow("LABEL $%substr_conv_end\n");
	tmpFile_addRow("JUMPIFEQ $%Itoint LF@%I_type string@float\n");
	tmpFile_addRow("JUMPIFEQ $%Ntoint LF@%N_type string@float\n");
	tmpFile_addRow("LABEL $%substr_type_end\n");
	tmpFile_addRow("LT LF@%substr_res LF@%1 int@0\n");
	tmpFile_addRow("JUMPIFEQ $%substr_end LF@%substr_res bool@true\n");
	tmpFile_addRow("GT LF@%substr_res LF@%1 int@0\n");
	tmpFile_addRow("JUMPIFEQ $%substr_end LF@%substr_res bool@true\n"); 
	tmpFile_addRow("LABEL $%substr_while_start\n");
	tmpFile_addRow("JUMPIFEQ $%substr_end LF@%str_len LF@%1\n");
	tmpFile_addRow("JUMPIFEQ $%substr_end LF@%1 LF@%2\n");
	tmpFile_addRow("GETCHAR LF@%substr_res LF@%0 LF@%1\n");
	tmpFile_addRow("WRITE LF@%substr_res\n");
	tmpFile_addRow("JUMP $%substr_while_start\n");
	tmpFile_addRow("LABEL $%substr_end\n");
	genCode_PopFrame();
	tmpFile_addRow("RETURN\n");
}

/*generates code for IFJ20 functions ord*/
void genCode_ORD() {
	tmpFile_addRow("DEFVAR LF@%type\n");
	tmpFile_addRow("TYPE LF@%type LF@%0\n");  
	tmpFile_addRow("JUMPIFNEQ $%EXIT LF@%type string@string\n");
	tmpFile_addRow("TYPE LF@%type LF@%1\n");
	tmpFile_addRow("JUMPIFNEQ $%ord_T1_nf LF@%type string@float\n");
	tmpFile_addRow("FLOAT2INT LF@%1 LF@%1\n");
	tmpFile_addRow("LABEL $%ord_T1_nf\n");
	tmpFile_addRow("JUMPIFNEQ $%EXIT LF@%type string@int\n");
	tmpFile_addRow("DEFVAR LF@%retval\n");
	tmpFile_addRow("MOVE LF@%retval nil@nil\n");
	tmpFile_addRow("DEFVAR LF@%str_len\n");
	tmpFile_addRow("STRLEN LF@%str_len LF@%0\n");
	tmpFile_addRow("SUB LF@%str_len LF@%str_len int@1\n");
	tmpFile_addRow("DEFVAR LF@%char\n");  
	tmpFile_addRow("GT LF@%ord_result LF@%1 LF@%str_len\n");
	tmpFile_addRow("JUMPIFEQ $%substr_end LF@%ord_result bool@true\n");
	tmpFile_addRow("LT LF@%ord_result LF@%1 int@0\n");
	tmpFile_addRow("JUMPIFEQ $%substr_end LF@%ord_result bool@true\n");
	tmpFile_addRow("GETCHAR LF@%char LF@%0 LF@%1\n");
	tmpFile_addRow("STRI2INT LF@%char LF@%char int@0\n");
	tmpFile_addRow("MOVE LF@%retval LF@%char\n");
	tmpFile_addRow("LABEL $%substr_end\n");
	genCode_PopFrame();
	tmpFile_addRow("RETURN\n");
}

/*generates code for IFJ20 functions chr*/
void genCode_CHR() {
	tmpFile_addRow("DEFVAR LF@%type\n");
	tmpFile_addRow("TYPE LF@%type LF@%0\n");
	tmpFile_addRow("JUMPIFNEQ $%EXIT LF@%type string@int\n");
	tmpFile_addRow("DEFVAR LF@%retval\n");
	tmpFile_addRow("DEFVAR LF@%chr_result\n");
	tmpFile_addRow("MOVE LF@%retval nil@nil\n");
	tmpFile_addRow("GT LF@%chr_result LF@%0 int@255\n");
	tmpFile_addRow("JUMPIFEQ $%EXIT LF@%chr_result bool@true\n");
	tmpFile_addRow("LT LF@%chr_result LF@%0 int@0\n");
	tmpFile_addRow("JUMPIFEQ $%EXIT LF@%chr_result bool@true\n");
	tmpFile_addRow("INT2CHAR LF@%retval LF@%0\n");
	genCode_PopFrame();
	tmpFile_addRow("RETURN\n");
}

void genCode_Header() {
	tmpFile_addRow("# Start of Program\n");
	tmpFile_addRow(".IFJcode20\n");
	tmpFile_addRow("DEFVAR GF@%tmp\n");
	tmpFile_addRow("DEFVAR GF@%tmp1\n");
	tmpFile_addRow("DEFVAR GF@%tmp2\n");
	tmpFile_addRow("DEFVAR GF@%tmp_type1\n");
	tmpFile_addRow("DEFVAR GF@%tmp_type2\n");
	tmpFile_addRow("DEFVAR GF@res\n");
	tmpFile_addRow("DEFVAR GF@res_eq\n");
	genCode_DefVar_Local("res_con");
	genCode_DefVar_Local("res_len");
	genCode_DefVar_Local("res_gchar");
	genCode_DefVar_Local("res_schar");
	tmpFile_addRow("JUMP $$main\n");
}

void genCode_MainStart() {
	tmpFile_addRow("LABEL $$main\n");
	genCode_CreateFrame();
	genCode_PushFrame();
}

void genCode_MainEnd() {
	genCode_PopFrame();
	tmpFile_addRow("CLEARS\n");
}
/*-----END OF PREFUNCTION-----*/

/*-----STRING OPERATIONS-----*/

void genCode_CONCATS() {
	tmpFile_addRow("POPS GF@%tmp1\n");
	tmpFile_addRow("POPS GF@%tmp2\n");
	tmpFile_addRow("CONCAT LF@res_con GF@%tmp1 GF@%tmp2\n");
	tmpFile_addRow("PUSHS LF@res_con\n");
}

/*STRLEN*/
void genCode_STRLENS() {
	tmpFile_addRow("POPS GF@%tmp1\n");
	tmpFile_addRow("STRLEN LF@res_len LF@tm1_string\n");
	tmpFile_addRow("PUSHS LF@res_len\n");
}


void genCode_GETCHARS() {
	tmpFile_addRow("POPS GF@%tmp1\n");
	tmpFile_addRow("POPS GF@%tmp2\n");
	tmpFile_addRow("GETCHAR LF@res_gchar GF@%tmp2 GF@%tmp1\n");
	tmpFile_addRow("PUSHS LF@res_gchar\n");
}

void genCode_SETCHARS() {
	tmpFile_addRow("POPS GF@%tmp1\n");
	tmpFile_addRow("POPS GF@%tmp2\n");
	tmpFile_addRow("SETCHAR LF@res_schar GF@tmp2 GF@tmp1\n");
	tmpFile_addRow("PUSHS LF@res_schar\n");
}

/*-----MATHEMATICS OPERATIONS-----*/
void genCode_PushInt(char* a) {
	tmpFile_addRow("PUSHS int@");
	tmpFile_addRow(a);
	tmpFile_addRow("\n");
}
void genCode_PushFloat(char* a) {
	tmpFile_addRow("PUSHS float@");
	tmpFile_addRow(a);
	tmpFile_addRow("\n");
}
void genCode_PushBool(char* a) {
	tmpFile_addRow("PUSHS bool@");
	tmpFile_addRow(a);
	tmpFile_addRow("\n");
}
void genCode_PushString(char* a) {
	tmpFile_addRow("PUSHS string@");
	tmpFile_addRow(a);
	tmpFile_addRow("\n");
}

void genCode_PushVar(char* name,Token token) {
	tmpFile_addRow("MOVE LF@");
	tmpFile_addRow(name);
	switch (token.type)
	{
	case DataTypeInt: {
		tmpFile_addRow(" int@");
		if (token.attribute.value != NULL) {
			tmpFile_addRow(token.attribute.value);
		}
		break;
	}
	case DataTypeDouble: {
		tmpFile_addRow(" float@");
		if (token.attribute.value != NULL) {
			tmpFile_addRow(token.attribute.value);
		}
		break;
	}
	case DataTypeString: {
		tmpFile_addRow(" string@");
		if (token.attribute.value != NULL) {
			tmpFile_addRow(token.attribute.value);
		}
		break;
	}
	case DataTypeBool: {
		tmpFile_addRow(" bool@");
		if (token.attribute.value != NULL) {
			tmpFile_addRow(token.attribute.value);
		}
		break;
	}
	}
	tmpFile_addRow("\n");
	tmpFile_addRow("PUSHS LF@");
	tmpFile_addRow(name);
	tmpFile_addRow("\n");
}

/*generates code for plus*/
void genCode_ADD() {
	tmpFile_addRow("ADDS\n");
}

/*generates code for minus*/
void genCode_SUB() {
	tmpFile_addRow("SUBS\n");//needs to try which symbol is first (2-1 or 1-2)
}

/*generates code for mul*/
void genCode_MUL() {
	tmpFile_addRow("MULS\n");
}

/*generates code for div*/
void genCode_DIV() {//needs to try which symbol is first (2-1 or 1-2)
	tmpFile_addRow("DIVS\n");//maybe i could check NULL DIVISION??
}

/*generates code for idiv*/
void genCode_IDIV() {//needs to try which symbol is first (2-1 or 1-2)
	tmpFile_addRow("IDIVS\n");//maybe i could check NULL DIVISION??
}

/*generates code for and*/
void genCode_AND() {
	tmpFile_addRow("ANDS\n");
}

/*generates code for or*/
void genCode_OR() {
	tmpFile_addRow("ORS\n");
}

/*generates code for not*/
void genCode_NOT() {
	tmpFile_addRow("NOTS\n");
}

/*generates code for int2float*/
void genCode_Int2Float() {
	tmpFile_addRow("INT2FLOATS\n");
}

/*generates code for float2int*/
void genCode_Float2Int() {
	tmpFile_addRow("FLOAT2INTS\n");
}

/*generates code for int2char*/
void genCode_Int2Char() {
	tmpFile_addRow("INT2CHARS\n");
}

/*generates code for str2int*/
void genCode_Str2Int() {
	tmpFile_addRow("STR2INTS\n");
}

/*-----END OF MATHEMATICS OPERATIONS-----*/


/*-----CONDITIONS-----*/
/*Generates code for compare variables for conditions*/
void genCode_Compare(Rule r) {
	if (this_is_for) {
		strcat(fid, "forid_com");
		sprintf(fid, "%d", idfor);
		genCode_DefVar_Global(fid);
		switch (r) {
		case RuleLess: {
			tmpFile_addRow("LTS\n");//needs to try which symbol is first (2 < 1 or 1 < 2)
			tmpFile_addRow("POPS GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow("\n");
			break;
		}
		case RuleMore: {
			tmpFile_addRow("GTS\n");//needs to try which symbol is first (2 > 1 or 1 > 2)
			tmpFile_addRow("POPS GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow("\n");
			break;
		}
		case RuleLessEqual: {//needs to try which symbol is first (2 <= 1 or 1 <= 2)
			tmpFile_addRow("POPS GF@%tmp2\n");
			tmpFile_addRow("POPS GF@%tmp1\n");
			tmpFile_addRow("LT GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow(" GF@tmp1 GF@tmp2\n");
			tmpFile_addRow("EQ GF@res_eq GF@tmp1 GF@tmp2\n");
			tmpFile_addRow("OR GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow(" GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow(" GF@res_eq\n");
			break;
		}
		case RuleMoreEqual: {//needs to try which symbol is first (2 >= 1 or 1 >= 2)
			tmpFile_addRow("POPS GF@%tmp2\n");
			tmpFile_addRow("POPS GF@%tmp1\n");
			tmpFile_addRow("GT GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow(" GF@tmp1 GF@tmp2\n");
			tmpFile_addRow("EQ GF@res_eq GF@tmp1 GF@tmp2\n");
			tmpFile_addRow("OR GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow(" GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow(" GF@res_eq\n");
			break;
		}
		case RuleEqual: {
			tmpFile_addRow("EQS\n");
			tmpFile_addRow("POPS GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow("\n");
			break;
		}
		case RuleNotEqual: {
			tmpFile_addRow("EQS\n");
			tmpFile_addRow("POPS GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow("\n");
			tmpFile_addRow("NOT GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow(" GF@");
			tmpFile_addRow(fid);
			tmpFile_addRow("\n");
			break;
		}
		}
		fid = "";
	}
	switch (r) {
	case RuleLess: {
		tmpFile_addRow("LTS\n");//needs to try which symbol is first (2 < 1 or 1 < 2)
		tmpFile_addRow("POPS GF@res\n");
		break;
	}
	case RuleMore: {
		tmpFile_addRow("GTS\n");//needs to try which symbol is first (2 > 1 or 1 > 2)
		tmpFile_addRow("POPS GF@res\n");
		break;
	}
	case RuleLessEqual: {//needs to try which symbol is first (2 <= 1 or 1 <= 2)
		tmpFile_addRow("POPS GF@%tmp2\n");
		tmpFile_addRow("POPS GF@%tmp1\n");
		tmpFile_addRow("LT GF@res GF@tmp1 GF@tmp2\n");
		tmpFile_addRow("EQ GF@res_eq GF@tmp1 GF@tmp2\n");
		tmpFile_addRow("OR GF@res GF@res GF@res_eq\n");
		break;
	}
	case RuleMoreEqual: {//needs to try which symbol is first (2 >= 1 or 1 >= 2)
		tmpFile_addRow("POPS GF@%tmp2\n");
		tmpFile_addRow("POPS GF@%tmp1\n");
		tmpFile_addRow("GT GF@res GF@tmp1 GF@tmp2\n");
		tmpFile_addRow("EQ GF@res_eq GF@tmp1 GF@tmp2\n");
		tmpFile_addRow("OR GF@res GF@res GF@res_eq\n");
		break;
	}
	case RuleEqual: {
		tmpFile_addRow("EQS\n");
		tmpFile_addRow("POPS GF@res\n");
		break;
	}
	case RuleNotEqual: {
		tmpFile_addRow("EQS\n");
		tmpFile_addRow("POPS GF@res\n");
		tmpFile_addRow("NOT GF@res GF@res\n");
		break;
	}
	}
}


/*Generates code for start condition*/
void genCode_if_MainStart(Rule r) {
	idif++;
	tmpFile_addRow("JUMPIFNEQ");
	tmpFile_addRow(" $if_else");
	sprintf(ids, "%d", idif);
	tmpFile_addRow(ids);
	ids = "";
	tmpFile_addRow(" GF@res bool@true\n");
}

/*Generates code for else*/
void generate_if_else() {
	tmpFile_addRow("JUMP");
	tmpFile_addRow(" $if_end");
	sprintf(ids, "%d", idif);
	tmpFile_addRow(ids);
	tmpFile_addRow("\n");
	tmpFile_addRow("LABEL ");
	tmpFile_addRow("$if_else");
	tmpFile_addRow(ids);
	tmpFile_addRow("\n");
	ids = "";
}

/*Generates code for end of condition*/
void genCode_if_EndLabel() {
	tmpFile_addRow("LABEL");
	tmpFile_addRow(" $if_end");
	sprintf(ids, "%d", idif);
	tmpFile_addRow(ids);
	tmpFile_addRow("\n");
	ids = "";
	idif--;
}
/*-----END OF CONDITION-----*/

/*-----FOR CYCLE-----*/

/*Generates code for cycle start*/
void genCode_ForStart() {
	idfor++;
	tmpFile_addRow("LABEL");
	tmpFile_addRow(" $for_");
	sprintf(fid, "%d", idfor);
	tmpFile_addRow(fid);
	tmpFile_addRow("\n");
	this_is_for = true;
	fid = "";
}

void genCode_ForIncStart() {//call after generating start of for
	for_inc = true;
}


void genCode_ForIncEnd() {//call before generating body of for
	for_inc = false;
}

void genCode_ForJump() {//call after ForIncEnd
	this_is_for = false;
	sprintf(fid, "%d", idfor);
	char*idsa = fid;
	strcat(fid, "forid_com");
	tmpFile_addRow("JUMPIFNEQ $for_end");
	tmpFile_addRow(idsa);
	tmpFile_addRow(" GF@");
	tmpFile_addRow(fid);
	tmpFile_addRow(" bool@true\n");
	tmpFile_addRow(idsa);
	fid = "";
}

/*Generates code for cycle end*/
void genCode_ForEnd() {
	tmpFile_addRow(inc_for);
	inc_for = "";
	tmpFile_addRow("JUMP $for_");
	sprintf(fid, "%d", idfor);
	tmpFile_addRow(fid);
	tmpFile_addRow("\n");
	tmpFile_addRow("LABEL");
	tmpFile_addRow(" $for_end");
	tmpFile_addRow(fid);
	tmpFile_addRow("\n");
	idfor--;
	fid = "";
}