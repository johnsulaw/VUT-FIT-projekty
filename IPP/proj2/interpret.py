#!/usr/bin/env python3

# IPP: 2. projekt - interpret.py
# autor: Jan Šulavík (xsulav01)

import sys

from interpret_classes.argument_parser import ArgumentParser
from interpret_classes.source_parser import SourceParser
from interpret_classes.instruction_handler import InstructionHandler
from interpret_classes.frames import Frames
from interpret_classes.data_stack import DataStack


def main():
    argument_parser = ArgumentParser()
    src_name, input_name = argument_parser.check()

    instruction_handler = InstructionHandler()
    my_stdin = sys.stdin

    if src_name != "stdin" and input_name != "stdin":
        src_file = open(src_name, "r")
        sys.stdin = open(input_name, "r")
    elif src_name != "stdin" and input_name == "stdin":
        src_file = open(src_name, "r")
        input_file = sys.stdin
    elif src_name == "stdin" and input_name != "stdin":
        src_file = sys.stdin
        input_file = open(input_name, "r")

    source_parser = SourceParser(src_file, instruction_handler)

    # check validity of source xml
    source_parser.checkXMLformat()
    source_parser.checkXMLsyntax()

    frames = Frames()
    data_stack = DataStack()

    # sort the instruction list by instruction order
    instruction_handler.instruction_list.sort(key=lambda x: x.order)

    # main loop
    while True:
        instr = instruction_handler.get_instruction()

        if instr is None:
            break
        elif instr.opcode == 'MOVE':
            var1_frame, var1_name = frames.get_variable_name_and_frame(instr.arg1)
            var1_frame = frames.get_frame(var1_frame)

            frames.declaration_check(var1_name, var1_frame)

            sym_type, sym_value, pointer = frames.get_arg_type_and_value(instr.arg2)
            if pointer == "var":
                var2_frame, var2_name = frames.get_variable_name_and_frame(instr.arg2)
                var2_frame = frames.get_frame(var2_frame)

                frames.declaration_check(var2_name, var2_frame)

                var2_value = var2_frame.get(var2_name)
                frames.update_var_value(var1_name, var1_frame, var2_value)
            else:
                value = instr.arg2['value']
                if instr.arg2['type'] == "int":
                    frames.update_var_value(var1_name, var1_frame, int(value))
                else:
                    frames.update_var_value(var1_name, var1_frame, value)

        elif instr.opcode == 'CREATEFRAME':
            frames.create_temp_frame()

        elif instr.opcode == 'PUSHFRAME':
            # check if tempFrame is defined, if true push tempFrame to frameStack
            if frames.tempFrame_defined:
                frames.push_to_frameStack(frames.tempFrame)
                frames.tempFrame_defined = False
            else:
                sys.stderr.write("Temporary frame is not defined!\n")
                exit(55)

        elif instr.opcode == 'POPFRAME':
            # pop local frame from frameStack and save its value into frame
            frames.tempFrame = frames.pop_from_frameStack()
            frames.tempFrame_defined = True

        elif instr.opcode == 'DEFVAR':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            frames.add_to_frame(var_name, var_frame)

        elif instr.opcode == 'CALL':
            instruction_handler.push_to_call_stack(instruction_handler.current_inst_number)

            # jump to label
            type, name, pointer = frames.get_arg_type_and_value(instr.arg1)
            if type == "label":
                if name in instruction_handler.label_dictionary:
                    instruction_handler.current_inst_number = instruction_handler.label_dictionary.get(name)
                else:
                    sys.stderr.write("Label does not exist!\n")
                    exit(52)

        elif instr.opcode == 'RETURN':
            inst_number = instruction_handler.pop_from_call_stack()
            instruction_handler.current_inst_number = inst_number

        elif instr.opcode == 'PUSHS':
            sym_type, sym_value, pointer = frames.get_arg_type_and_value(instr.arg1)

            if pointer == "value":
                data_stack.push_to_data_stack(sym_value)
            elif pointer == "var":
                var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
                var_frame = frames.get_frame(var_frame)

                frames.declaration_check(var_name, var_frame)
                value = var_frame.get(var_name)
                if value is None:
                    sys.stderr.write("Pushing uninitialized variable into data stack")
                    exit(56)

                data_stack.push_to_data_stack(value)

        elif instr.opcode == 'POPS':
            value = data_stack.pop_from_data_stack()

            var_type, var_value, pointer = frames.get_arg_type_and_value(instr.arg1)
            if pointer != "var":
                sys.stderr.write("Bad argument for POPS!\n")
                exit(53)

            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            frames.update_var_value(var_name, var_frame, value)

        elif instr.opcode in ["ADD", "SUB", "MUL", "IDIV"]:
            var_type, var_name, pointer = frames.get_arg_type_and_value(instr.arg1)

            if var_type == "var":
                var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
                var_frame = frames.get_frame(var_frame)

                frames.declaration_check(var_name, var_frame)
            else:
                sys.stderr.write("First argument has to be variable!\n")
                exit(53)

            sym1_type, sym1_value, sym1_pointer = frames.get_arg_type_and_value(instr.arg2)
            sym2_type, sym2_value, sym2_pointer = frames.get_arg_type_and_value(instr.arg3)

            if sym1_pointer == "value":
                try:
                    operand1 = int(sym1_value)
                except ValueError:
                    sys.stderr.write("Wrong operand type!\n")
                    exit(53)
            else:
                op1_var_frame, op1_var_name = frames.get_variable_name_and_frame(instr.arg2)
                op1_var_frame = frames.get_frame(op1_var_frame)

                frames.declaration_check(op1_var_name, op1_var_frame)

                try:
                    operand1 = int(op1_var_frame.get(op1_var_name))
                except ValueError:
                    sys.stderr.write("Wrong operand type!\n")
                    exit(53)

            if sym2_pointer == "value":
                try:
                    operand2 = int(sym2_value)
                except ValueError:
                    sys.stderr.write("Wrong operand type!\n")
                    exit(53)
            else:
                op2_var_frame, op2_var_name = frames.get_variable_name_and_frame(instr.arg3)
                op2_var_frame = frames.get_frame(op2_var_frame)

                frames.declaration_check(op2_var_name, op2_var_frame)

                try:
                    operand2 = int(op2_var_frame.get(op2_var_name))
                except ValueError:
                    sys.stderr.write("Wrong operand type!\n")
                    exit(53)

            if instr.opcode == "ADD":
                result = operand1 + operand2
            elif instr.opcode == "SUB":
                result = operand1 - operand2
            elif instr.opcode == "MUL":
                result = operand1 * operand2
            elif instr.opcode == "IDIV":
                if operand2 == 0:
                    sys.stderr.write("Division by zero!\n")
                    exit(57)
                else:
                    result = operand1 // operand2

            frames.update_var_value(var_name, var_frame, result)

        elif instr.opcode in ["LT", "GT", "EQ"]:
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            frames.declaration_check(var_name, var_frame)

            sym1_type, sym1_value, sym1_pointer = frames.get_arg_type_and_value(instr.arg2)
            sym2_type, sym2_value, sym2_pointer = frames.get_arg_type_and_value(instr.arg3)

            if sym1_pointer == "value":
                operand1 = sym1_value
                operand1_type = sym1_type
            elif sym1_pointer == "var":
                sym1_var_frame, sym1_var_name = frames.get_variable_name_and_frame(instr.arg2)
                sym1_var_frame = frames.get_frame(sym1_var_frame)

                frames.declaration_check(sym1_var_name, sym1_var_frame)
                operand1 = sym1_var_frame.get(sym1_var_name)

                if isinstance(operand1, int):
                    operand1_type = "int"
                elif isinstance(operand1, bool):
                    operand1_type = "bool"
                elif isinstance(operand1, str):
                    operand1_type = "str"
                else:
                    sys.stderr.write("Bad operand type in LT/GT/EQ")
                    exit(53)

            if sym2_pointer == "value":
                operand2 = sym1_value
                operand2_type = sym2_type
            elif sym2_pointer == "var":
                sym2_var_frame, sym2_var_name = frames.get_variable_name_and_frame(instr.arg3)
                sym2_var_frame = frames.get_frame(sym2_var_frame)

                frames.declaration_check(sym2_var_name, sym2_var_frame)
                operand2 = sym2_var_frame.get(sym2_var_name)

                if isinstance(operand2, int):
                    operand2_type = "int"
                elif isinstance(operand2, bool):
                    operand2_type = "bool"
                elif isinstance(operand2, str):
                    operand2_type = "str"
                else:
                    sys.stderr.write("Bad operand type in LT/GT/EQ")
                    exit(53)

            if (operand1 == "nil" or operand2 == "nil") and instr.opcode != "EQ":
                sys.stderr.write("Wrong operand")
                exit(53)

            if operand1_type != operand2_type:
                sys.stderr.write("Operand types do not match in LT/GT\n")
                exit(53)

            if instr.opcode == "LT":
                if operand1 < operand2:
                    result = "true"
                else:
                    result = "false"
            elif instr.opcode == "GT":
                if operand1 > operand2:
                    result = "true"
                else:
                    result = "false"
            else:
                if operand1 == operand2:
                    result = "true"
                else:
                    result = "false"

            frames.update_var_value(var_name, var_frame, result)

        elif instr.opcode == 'AND' or instr.opcode == 'OR':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            frames.declaration_check(var_name, var_frame)

            sym1_type, sym1_value, sym1_pointer = frames.get_arg_type_and_value(instr.arg2)
            sym2_type, sym2_value, sym2_pointer = frames.get_arg_type_and_value(instr.arg3)

            if sym1_pointer == "value":
                operand1 = sym1_value
            elif sym1_pointer == "var":
                sym1_var_frame, sym1_var_name = frames.get_variable_name_and_frame(instr.arg2)
                sym1_var_frame = frames.get_frame(sym1_var_frame)

                frames.declaration_check(sym1_var_name, sym1_var_frame)
                operand1 = sym1_var_frame.get(sym1_var_name)

            if sym2_pointer == "value":
                operand2 = sym1_value
            elif sym2_pointer == "var":
                sym2_var_frame, sym2_var_name = frames.get_variable_name_and_frame(instr.arg3)
                sym2_var_frame = frames.get_frame(sym2_var_frame)

                frames.declaration_check(sym2_var_name, sym2_var_frame)
                operand2 = sym2_var_frame.get(sym2_var_name)

            if operand1 not in ['true', 'false'] or operand2 not in ['true', 'false']:
                sys.stderr.write("Bad operand values for logical AND/OR")
                exit(53)

            if instr.opcode == "AND":
                if operand1 == operand2 == 'true':
                    result = "true"
                else:
                    result = "false"
            else:
                if operand1 == 'true' or operand2 == 'true':
                    result = "true"
                else:
                    result = "false"

            frames.update_var_value(var_name, var_frame, result)

        elif instr.opcode == 'NOT':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            frames.declaration_check(var_name, var_frame)

            sym_type, sym_value, pointer = frames.get_arg_type_and_value(instr.arg2)

            if pointer == "value":
                operand1 = sym_value
            elif pointer == "var":
                sym_var_frame, sym_var_name = frames.get_variable_name_and_frame(instr.arg2)
                sym_var_frame = frames.get_frame(sym_var_frame)

                frames.declaration_check(sym_var_name, sym_var_frame)
                operand1 = sym_var_frame.get(sym_var_name)

            operand1 = operand1.lower()

            if operand1 not in ["false", "true"]:
                sys.stderr.write("Wrong operand type for NOT\n")
                exit(53)

            if operand1 == "true":
                result = "false"
            else:
                result = "true"

            frames.update_var_value(var_name, var_frame, result)

        elif instr.opcode == 'INT2CHAR':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)
            frames.declaration_check(var_name, var_frame)

            sym_type, sym_value, sym_pointer = frames.get_arg_type_and_value(instr.arg2)

            if sym_pointer == "value":
                if sym_type != "int":
                    sys.stderr.write("Bad type of symbol in INT2CHAR\n")
                    exit(53)

                value = sym_value
                string_value = chr(value)
                frames.update_var_value(var_name, var_frame, string_value)
            elif sym_pointer == "var":
                sym_var_frame, sym_var_name = frames.get_variable_name_and_frame(instr.arg2)
                sym_var_frame = frames.get_frame(sym_var_frame)

                frames.declaration_check(sym_var_name, sym_var_frame)

                try:
                    value = int(sym_var_frame.get(sym_var_name))
                except:
                    sys.stderr.write("Bad type of symbol in INT2CHAR\n")
                    exit(53)

                if value < 0:
                    sys.stderr.write("Wrong integer value in INT2CHAR")
                    exit(58)

                string_value = chr(value)
                frames.update_var_value(var_name, var_frame, string_value)

        elif instr.opcode == 'STRI2INT':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            frames.declaration_check(var_name, var_frame)

            sym1_type, sym1_value, sym1_pointer = frames.get_arg_type_and_value(instr.arg2)
            sym2_type, sym2_value, sym2_pointer = frames.get_arg_type_and_value(instr.arg3)

            if sym1_pointer == "value":
                operand1 = sym1_value
            elif sym1_pointer == "var":
                operand1_var_frame, operand1_var_name = frames.get_variable_name_and_frame(instr.arg2)
                operand1_var_frame = frames.get_frame(operand1_var_frame)

                frames.declaration_check(operand1_var_name, operand1_var_frame)
                operand1 = operand1_var_frame.get(operand1_var_name)

            if sym2_pointer == "value":
                operand2 = sym2_value
            elif sym2_pointer == "var":
                operand2_var_frame, operand2_var_name = frames.get_variable_name_and_frame(instr.arg3)
                operand2_var_frame = frames.get_frame(operand2_var_frame)

                frames.declaration_check(operand2_var_name, operand2_var_frame)
                operand2 = operand2_var_frame.get(operand2_var_name)

            try:
                operand2 = int(operand2)
            except:
                sys.stderr.write("Operand 2 in STRI2INT is not integer")
                exit(53)

            if not isinstance(operand1, str):
                sys.stderr.write("Operand 1 is STRI2INT is not string")
                exit(53)

            if len(operand1) - 1 >= operand2:
                result = operand1[operand2]
                result = ord(result)
            else:
                sys.stderr.write("Indexing out of bounds!\n")
                exit(58)

            frames.update_var_value(var_name, var_frame, result)

        elif instr.opcode == 'READ':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            frames.declaration_check(var_name, var_frame)

            type, value_type, pointer = frames.get_arg_type_and_value(instr.arg2)

            try:
                value = input()
            except:
                value = ''

            if value_type == "int":
                try:
                    value = int(value)
                except:
                    value = ''

                frames.update_var_value(var_name, var_frame, value)
            elif value_type == "string":
                try:
                    value = str(value)
                except:
                    value = ''

                frames.update_var_value(var_name, var_frame, value)
            elif value_type == "bool":
                if value.lower() == 'true':
                    value = "true"
                else:
                    value = "false"

                frames.update_var_value(var_name, var_frame, value)

        elif instr.opcode == 'WRITE':
            sym_type, sym_value, pointer = frames.get_arg_type_and_value(instr.arg1)

            if pointer == "var":
                var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
                var_frame = frames.get_frame(var_frame)

                frames.declaration_check(var_name, var_frame)
                value = var_frame.get(var_name)
                if value is None:
                    sys.stderr.write("Reading from uninitialized variable in WRITE")
                    exit(56)

                if value is "nil@nil":
                    print('', end='')
                else:
                    print(value, end='')

            elif pointer == "value":
                type = instr.arg1['type']
                value = instr.arg1['value']

                if type == "nil":
                    print('', end='')
                elif type in ["bool", "int", "string"]:
                    print(value, end='')

        elif instr.opcode == 'CONCAT':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            sym1_type, sym1_value, sym1_pointer = frames.get_arg_type_and_value(instr.arg2)
            sym2_type, sym2_value, sym2_pointer = frames.get_arg_type_and_value(instr.arg3)

            if sym1_pointer == "value":
                if sym1_type == "string":
                    operand1 = sym1_value
                else:
                    sys.stderr.write("Operands have to be string in CONCAT")
                    exit(53)
            elif sym1_pointer == "var":
                sym1_var_frame, sym1_var_name = frames.get_variable_name_and_frame(instr.arg2)
                sym1_var_frame = frames.get_frame(sym1_var_frame)

                operand1 = sym1_var_frame.get(sym1_var_name)

            if sym2_pointer == "value":
                if sym2_type == "string":
                    operand2 = sym2_value
                else:
                    sys.stderr.write("Operands have to be string in CONCAT")
                    exit(53)
            elif sym2_pointer == "var":
                sym2_var_frame, sym2_var_name = frames.get_variable_name_and_frame(instr.arg3)
                sym2_var_frame = frames.get_frame(sym2_var_frame)

                operand2 = sym2_var_frame.get(sym2_var_name)

            if operand1 is None or operand2 is None:
                sys.stderr.write("Passing uninitialized variables to CONCAT as arguments")
                exit(56)

            if isinstance(operand1, str) and isinstance(operand2, str):
                result = operand1 + operand2
            else:
                sys.stderr.write("Both operands have to be string type for instruction CONCAT")
                exit(53)

            frames.update_var_value(var_name, var_frame, result)

        elif instr.opcode == 'STRLEN':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            sym_type, sym_value, pointer = frames.get_arg_type_and_value(instr.arg2)
            if pointer == "value":
                if sym_type == "string":
                    result = len(sym_value)
                else:
                    sys.stderr.write("STRLEN needs string")
                    exit(53)
            elif pointer == "var":
                sym_var_frame, sym_var_name = frames.get_variable_name_and_frame(instr.arg2)
                sym_var_frame = frames.get_frame(sym_var_frame)

                result = sym_var_frame.get(sym_var_name)
                if result is None:
                    sys.stderr.write("Reading from uninitialized variable in STRLEN")
                    exit(56)
                if not isinstance(result, str):
                    sys.stderr.write("STRLEN needs string")
                    exit(53)

                result = len(result)

            frames.update_var_value(var_name, var_frame, result)

        elif instr.opcode == 'GETCHAR':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            sym1_type, sym1_value, sym1_pointer = frames.get_arg_type_and_value(instr.arg2)
            sym2_type, sym2_value, sym2_pointer = frames.get_arg_type_and_value(instr.arg3)
            if sym1_pointer == "value":
                if sym1_type == "string":
                    string = sym1_value
                else:
                    sys.stderr.write("GETCHAR symb1 needs to be string")
                    exit(53)
            elif sym1_pointer == "var":
                sym1_var_frame, sym1_var_name = frames.get_variable_name_and_frame(instr.arg2)
                sym1_var_frame = frames.get_frame(sym1_var_frame)

                string = sym1_var_frame.get(sym1_var_name)
                if string is None:
                    sys.stderr.write("Reading from uninitialized variable in GETCHAR")
                    exit(56)

                if not isinstance(string, str):
                    sys.stderr.write("GETCHAR symb1 needs to be string")
                    exit(53)

            if sym2_pointer == "value":
                if sym2_type == "int":
                    try:
                        index = int(sym2_value)
                    except TypeError:
                        sys.stderr.write("GETCHAR symb2 has to be int")
                        exit(53)
                else:
                    sys.stderr.write("GETCHAR symb2 has to be int")
                    exit(53)
            elif sym2_pointer == "var":
                sym2_var_frame, sym2_var_name = frames.get_variable_name_and_frame(instr.arg3)
                sym2_var_frame = frames.get_frame(sym2_var_frame)

                index = sym2_var_frame.get(sym2_var_name)
                if index is None:
                    sys.stderr.write("Reading from uninitialized variable in GETCHAR")
                    exit(56)
                try:
                    index = int(index)
                except:
                    sys.stderr.write("GETCHAR symb2 has to be int")
                    exit(53)

            if len(string) - 1 < index:
                sys.stderr.write("Index out of bounds for GECTHAR")
                exit(58)
            else:
                result = string[index]

            frames.update_var_value(var_name, var_frame, result)

        elif instr.opcode == 'SETCHAR':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            string_to_modify = var_frame.get(var_name)
            if not isinstance(string_to_modify, str):
                sys.stderr.write("SETCHAR needs string in <var>")
                exit(53)

            sym1_type, sym1_value, sym1_pointer = frames.get_arg_type_and_value(instr.arg2)
            sym2_type, sym2_value, sym2_pointer = frames.get_arg_type_and_value(instr.arg3)

            if sym1_pointer == "value":
                try:
                    index = int(sym1_value)
                except TypeError:
                    sys.stderr.write("SETCHAR needs symb1 to be int")
                    exit(53)
            elif sym1_pointer == "var":
                sym1_var_frame, sym1_var_name = frames.get_variable_name_and_frame(instr.arg2)
                sym1_var_frame = frames.get_frame(sym1_var_frame)

                index = sym1_var_frame.get(sym1_var_name)
                try:
                    index = int(index)
                except:
                    sys.stderr.write("SETCHAR needs symb1 to be int")
                    exit(53)

            if sym2_pointer == "value":
                if sym2_type == "string":
                    if sym2_value == "":
                        sys.stderr.write("Cannot pass empty string to SETCHAR\n")
                        exit(58)
                    if isinstance(sym2_value, str):
                        char = sym2_value[0]
                    else:
                        sys.stderr.write("SETCHAR needs symb2 to be string")
                        exit(53)
                else:
                    sys.stderr.write("SETCHAR needs symb2 to be string")
                    exit(53)
            elif sym2_pointer == "var":
                sym2_var_frame, sym2_var_name = frames.get_variable_name_and_frame(instr.arg3)
                sym2_var_frame = frames.get_frame(sym2_var_frame)

                string = sym2_var_frame.get(sym2_var_name)
                if string == "":
                    sys.stderr.write("Cannot pass empty string to SETCHAR\n")
                    exit(58)
                if not isinstance(string, str):
                    sys.stderr.write("SETCHAR needs symb2 to be string")
                    exit(53)

                char = string[0]

            if len(string_to_modify) - 1 < index:
                sys.stderr.write("Index out of bounds for GECTHAR")
                exit(58)
            else:
                string_to_modify = list(string_to_modify)
                string_to_modify[index] = char
                string_to_modify = "".join(string_to_modify)

            frames.update_var_value(var_name, var_frame, string_to_modify)

        elif instr.opcode == 'TYPE':
            var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
            var_frame = frames.get_frame(var_frame)

            frames.declaration_check(var_name, var_frame)

            symb_type, symb_value, pointer = frames.get_arg_type_and_value(instr.arg2)
            if symb_type is None:
                symb_type = ''

            if pointer == "var":
                symb_var_frame, symb_var_name = frames.get_variable_name_and_frame(instr.arg2)
                symb_var_frame = frames.get_frame(symb_var_frame)

                var_value = symb_var_frame.get(symb_var_name)
                if var_value is None:
                    symb_type = ''
                elif isinstance(var_value, int):
                    symb_type = "int"
                elif var_value == "nil":
                    symb_type = "nil"
                elif var_value in ['true', 'false']:
                    symb_type = "bool"
                else:
                    symb_type = "string"

            frames.update_var_value(var_name, var_frame, symb_type)

        elif instr.opcode == 'LABEL':
            continue

        elif instr.opcode == 'JUMP':
            type, name, pointer = frames.get_arg_type_and_value(instr.arg1)
            if type == "label":
                if name in instruction_handler.label_dictionary:
                    instruction_handler.current_inst_number = instruction_handler.label_dictionary.get(name)
                else:
                    sys.stderr.write("Label does not exist!\n")
                    exit(52)

        elif instr.opcode in ['JUMPIFEQ', 'JUMPIFNEQ']:
            label_type, label_value, label_pointer = frames.get_arg_type_and_value(instr.arg1)
            symb1_type, symb1_value, symb1_pointer = frames.get_arg_type_and_value(instr.arg2)
            symb2_type, symb2_value, symb2_pointer = frames.get_arg_type_and_value(instr.arg3)

            if label_type == "label":
                if not label_value in instruction_handler.label_dictionary:
                    sys.stderr.write("Label does not exist!\n")
                    exit(52)

                if symb1_pointer == "var":
                    var1_frame, var1_name = frames.get_variable_name_and_frame(instr.arg2)
                    var1_frame = frames.get_frame(var1_frame)

                    frames.declaration_check(var1_name, var1_frame)

                    operand1 = var1_frame.get(var1_name)
                else:
                    operand1 = symb1_value

                if symb2_pointer == "var":
                    var2_frame, var2_name = frames.get_variable_name_and_frame(instr.arg3)
                    var2_frame = frames.get_frame(var2_frame)

                    frames.declaration_check(var2_name, var2_frame)

                    operand2 = var2_frame.get(var2_name)
                else:
                    operand2 = symb2_value

                if symb1_type == symb2_type or symb1_type == "nil" or symb2_type == "nil":
                    pass
                else:
                    sys.stderr.write("Operand types do not match in JUMPIFEQ instruction!\n")
                    exit(53)

                if symb1_type != "nil":
                    if isinstance(operand1, int):
                        symb1_type = "int"
                    elif isinstance(operand1, bool):
                        symb1_type = "bool"
                    elif isinstance(operand1, str):
                        symb1_type = "str"

                if symb2_type != "nil":
                    if isinstance(operand2, int):
                        symb2_type = "int"
                    elif isinstance(operand2, bool):
                        symb2_type = "bool"
                    elif isinstance(operand2, str):
                        symb2_type = "str"

                if symb1_type != symb2_type and (symb1_type != "nil" or symb2_type != "nil"):
                    sys.stderr.write("Operand types do not match in JUMPIFEQ/JUMPIFNEQ")
                    exit(53)
            else:
                sys.stderr.write("Arg1 has to be label in JUMPIFEQ/JUMPIFNEQ")
                exit(53)

            if instr.opcode == "JUMPIFEQ":
                if symb1_type == "int":
                    if int(operand1) == int(operand2):
                        instruction_handler.current_inst_number = instruction_handler.label_dictionary.get(label_value)
                    else:
                        continue
                elif symb1_type == "bool":
                    if bool(operand1) == bool(operand2):
                        instruction_handler.current_inst_number = instruction_handler.label_dictionary.get(label_value)
                    else:
                        continue
                elif symb1_type == "str":
                    if str(operand1) == str(operand2):
                        instruction_handler.current_inst_number = instruction_handler.label_dictionary.get(label_value)
                    else:
                        continue
                elif symb1_type == "nil":
                    if operand1 == operand2:
                        instruction_handler.current_inst_number = instruction_handler.label_dictionary.get(label_value)
                    else:
                        continue
            elif instr.opcode == "JUMPIFNEQ":
                if symb1_type == "int":
                    if int(operand1) != int(operand2):
                        instruction_handler.current_inst_number = instruction_handler.label_dictionary.get(label_value)
                    else:
                        continue
                elif symb1_type == "bool":
                    if bool(operand1) == bool(operand2):
                        instruction_handler.current_inst_number = instruction_handler.label_dictionary.get(label_value)
                    else:
                        continue
                elif symb1_type == "str":
                    if str(operand1) == str(operand2):
                        instruction_handler.current_inst_number = instruction_handler.label_dictionary.get(label_value)
                    else:
                        continue
                elif symb1_type == "nil":
                    if operand1 == operand2:
                        instruction_handler.current_inst_number = instruction_handler.label_dictionary.get(label_value)
                    else:
                        continue

        elif instr.opcode == 'EXIT':
            sym_type, sym_value, pointer = frames.get_arg_type_and_value(instr.arg1)

            if pointer == "value":
                result = sym_value
            else:
                var_frame, var_name = frames.get_variable_name_and_frame(instr.arg1)
                var_frame = frames.get_frame(var_frame)

                frames.declaration_check(var_name, var_frame)
                result = var_frame.get(var_name)
                if result is None:
                    sys.stderr.write("Reading uninitialized variable in EXIT")
                    exit(56)

            try:
                result = int(result)
            except TypeError:
                sys.stderr.write("Bad return code in instruction EXIT!")
                exit(57)

            if result < 0 or result > 49:
                sys.stderr.write("Bad return code in instruction EXIT!")
                exit(57)

            exit(result)

        elif instr.opcode == 'DPRINT':
            sym_type, sym_value, pointer = frames.get_arg_type_and_value(instr.arg1)

            if pointer == "value":
                result = sym_value
            else:
                var_frame, var_name = frames.get_variable_name_and_frame(sym_value)
                var_frame = frames.get_frame(var_frame)

                frames.declaration_check(var_name, var_frame)
                result = var_frame.get(var_name)

            # print to stderr
            sys.stderr.write(result)
        elif instr.opcode == 'BREAK':
            sys.stderr.write("instruction {} interpreted!\n".format(instr.opcode))
        else:
            sys.stderr.write("Unknown instruction {}!\n".format(instr.opcode))
            exit(32)


if __name__ == '__main__':
    main()
