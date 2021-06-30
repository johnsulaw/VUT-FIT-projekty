#!/usr/bin/env python3

# IPP: 2. projekt - source_parser.py
# autor: Jan Šulavík (xsulav01)


import xml.etree.ElementTree as ET
import sys
import re

from .instruction import Instruction


class SourceParser:
    def __init__(self, srcFile, instruction_loader):
        self.srcFile = srcFile
        self.instruction_list = instruction_loader
        self.instruction_order = 0
        self.instruction_order_arr = []

        try:
            tree = ET.parse(self.srcFile)
            self.program = tree.getroot()
        except:
            sys.stderr.write("Bad formatting of XML file!")
            exit(31)


    def checkXMLformat(self):
        if self.program.tag != 'program':
            sys.stderr.write("Bad root element!\n")
            exit(32)

        for attribute in self.program.attrib:
            if attribute not in ['language', 'name', 'description']:
                sys.stderr.write("Wrong program element attributes!\n")
                exit(32)

        # check if file is empty, or if it has any unwanted text
        if self.program.text is None:
            pass
        elif not self.program.text.isspace():
            sys.stderr.write("Excess text in program!")
            exit(32)

        if self.program.attrib['language'].upper() != "IPPCODE20":
            sys.stderr.write("Wrong value of language attribute!")
            exit(32)

        for instruction in self.program:
            if instruction.tag != 'instruction':
                sys.stderr.write("Wrong instruction element!\n")
                exit(32)
            if 'opcode' not in instruction.attrib or 'order' not in instruction.attrib:
                sys.stderr.write("Wrong instruction attributes!\n")
                exit(32)

            if instruction.text:
                if not instruction.text.isspace():
                    sys.stderr.write("Excess text in instruction!")
                    exit(32)

            for attribute in instruction.attrib:
                if attribute not in ['opcode', 'order']:
                    sys.stderr.write("Wrong attribute of instruction")
                    exit(32)

            try:
                int(instruction.attrib['order'])
            except ValueError:
                sys.stderr.write("Attribute order of instruction is not int!\n")
                exit(32)

            if int(instruction.attrib['order']) < 0:
                sys.stderr.write("Wrong value of attribute order!\n")
                exit(32)
            else:
                if int(instruction.attrib['order']) in self.instruction_order_arr:
                    sys.stderr.write("Duplicit instruction order!\n")
                    exit(32)
                else:
                    self.instruction_order_arr.append(int(instruction.attrib['order']))

            argNumber = 0
            for argument in instruction:

                for attribute in argument.attrib:
                    if attribute not in ['type']:
                        sys.stderr.write("Wrong attribute of argument")
                        exit(32)

                if len(argument):
                    sys.stderr.write("Argument cannot have own elements")
                    exit(32)

                argNumber = argNumber + 1
                if argument.tag != 'arg1' and argument.tag != 'arg2' and argument.tag != 'arg3':
                    sys.stderr.write("Wrong argument number!\n")
                    exit(32)
                if 'type' not in argument.attrib:
                    sys.stderr.write("Wrong argument attribute!\n")
                    exit(32)
                if argument.attrib['type'] not in ['bool', 'string', 'int', 'var', 'label', 'nil', 'type']:
                    sys.stderr.write("Wrong argument type!\n")
                    exit(32)

            # checking if argument tags have proper names in relation to argument number
            if argNumber == 1:
                for argument in instruction:
                    if argument.tag != "arg1":
                        sys.stderr.write("Wrong argument number!")
                        exit(32)
            elif argNumber == 2:
                for argument in instruction:
                    if argument.tag != "arg2" and argument.tag != "arg1":
                        sys.stderr.write("Wrong argument number!")
                        exit(32)
            elif argNumber == 3:
                for argument in instruction:
                    if argument.tag != "arg3" and argument.tag != "arg2" and argument.tag != "arg1":
                        sys.stderr.write("Wrong argument number!")
                        exit(32)

    def checkXMLsyntax(self):
        for instruction in self.program:
            argument_array = []
            for argument in instruction:
                argument_array.append(argument)

            # sorting the argument array, so the arguments come in expected order
            argument_array.sort(key=lambda x: x.tag)
            opcode = instruction.attrib['opcode'].upper()

            if opcode in ["MOVE", "NOT", "STRLEN", "TYPE", "INT2CHAR"]:
                if len(argument_array) != 2:
                    sys.stderr.write("wrong syntax\n")
                    exit(32)

                argument_array[0] = self.check_var(argument_array[0])
                argument_array[1] = self.check_symbol(argument_array[1])

                self.instruction_order = int(instruction.attrib['order'])
                i = Instruction(opcode.upper(), self.instruction_order, arg1=argument_array[0], arg2=argument_array[1])
                self.instruction_list.add_to_list(i)

            elif opcode in ["CREATEFRAME", "PUSHFRAME", "POPFRAME", "BREAK", "RETURN"]:
                if len(argument_array) != 0:
                    sys.stderr.write("wrong syntax\n")
                    exit(32)

                self.instruction_order = int(instruction.attrib['order'])
                i = Instruction(opcode.upper(), self.instruction_order)
                self.instruction_list.add_to_list(i)

            elif opcode in ["DEFVAR", "POPS"]:
                if len(argument_array) != 1:
                    sys.stderr.write("wrong syntax\n")
                    exit(32)

                argument_array[0] = self.check_var(argument_array[0])

                self.instruction_order = int(instruction.attrib['order'])
                i = Instruction(opcode.upper(), self.instruction_order, arg1=argument_array[0])
                self.instruction_list.add_to_list(i)

            elif opcode in ["LABEL", "JUMP", "CALL"]:
                if len(argument_array) != 1:
                    sys.stderr.write("wrong syntax\n")
                    exit(32)

                argument_array[0] = self.check_label(argument_array[0])

                self.instruction_order = int(instruction.attrib['order'])
                i = Instruction(opcode.upper(), self.instruction_order, arg1=argument_array[0])
                self.instruction_list.add_to_list(i)

            elif opcode in ["WRITE", "EXIT", "DPRINT", "PUSHS"]:
                if len(argument_array) != 1:
                    sys.stderr.write("wrong syntax\n")
                    exit(32)

                argument_array[0] = self.check_symbol(argument_array[0])

                self.instruction_order = int(instruction.attrib['order'])
                i = Instruction(opcode.upper(), self.instruction_order, arg1=argument_array[0])
                self.instruction_list.add_to_list(i)

            elif opcode in ["ADD", "SUB", "MUL", "IDIV", "LT", "GT", "EQ", "AND", "OR", "CONCAT",
                          "GETCHAR", "SETCHAR", "STRI2INT"]:
                if len(argument_array) != 3:
                    sys.stderr.write("wrong syntax\n")
                    exit(32)

                argument_array[0] = self.check_var(argument_array[0])
                argument_array[1] = self.check_symbol(argument_array[1])
                argument_array[2] = self.check_symbol(argument_array[2])

                self.instruction_order = int(instruction.attrib['order'])
                i = Instruction(opcode.upper(), self.instruction_order, arg1=argument_array[0], arg2=argument_array[1],
                                arg3=argument_array[2])
                self.instruction_list.add_to_list(i)

            elif opcode in ["READ"]:
                if len(argument_array) != 2:
                    sys.stderr.write("wrong syntax\n")
                    exit(32)

                argument_array[0] = self.check_var(argument_array[0])
                argument_array[1] = self.check_type(argument_array[1])

                self.instruction_order = int(instruction.attrib['order'])
                i = Instruction(opcode.upper(), self.instruction_order, arg1=argument_array[0], arg2=argument_array[1])
                self.instruction_list.add_to_list(i)

            elif opcode in ["JUMPIFEQ", "JUMPIFNEQ"]:
                if len(argument_array) != 3:
                    sys.stderr.write("wrong syntax\n")
                    exit(32)

                argument_array[0] = self.check_label(argument_array[0])
                argument_array[1] = self.check_symbol(argument_array[1])
                argument_array[2] = self.check_symbol(argument_array[2])

                self.instruction_order = int(instruction.attrib['order'])
                i = Instruction(opcode.upper(), self.instruction_order, arg1=argument_array[0], arg2=argument_array[1], arg3=argument_array[2])
                self.instruction_list.add_to_list(i)

            else:
                sys.stderr.write("Unknown instruction {}!\n".format(opcode))
                exit(32)

    def check_var(self, var):
        var_value = var.text

        pattern = re.compile(r'^(GF|LF|TF)@[A-Za-z|0-9|_|\-|$|&|%|*|!|?]+$')
        if pattern.search(var_value) is None:
            sys.stderr.writeint("wrong variable syntax\n")
            exit(32)
        return var

    def check_label(self, label):
        label_value = label.text

        pattern = re.compile(r'^[A-Za-z]|[0-9]|_|-|$|&|%|\*|!|\?]+$')
        if pattern.search(label_value) is None:
            sys.stderr.write("wrong label syntax\n")
            exit(32)
        return label

    def check_type(self, type):
        type_value = type.text

        pattern = re.compile(r'^(int|string|bool|nil)$')
        if pattern.search(type_value) is None:
            sys.stderr.write("wrong label syntax\n")
            exit(32)
        return type

    def check_symbol(self, symbol):
        if symbol.attrib['type'] == "label":
            symbol = self.check_label(symbol)
        elif symbol.attrib['type'] == "var":
            symbol = self.check_var(symbol)
        else:
            symbol = self.check_value(symbol)
        return symbol

    def check_value(self, symbol):
        if symbol.attrib['type'] not in ["string", "int", "bool", "nil"]:
            sys.stderr.write("Wrong value type.")
            exit(32)
        if symbol.attrib['type'] == "string":
            if symbol.text:
                if "#" in symbol.text or " " in symbol.text:
                    sys.stderr.write("The '#' and whitespace symbols not supported in string type.")
                    exit(32)

                if isinstance(symbol.text, str):
                    value = symbol.text
                    index = 0
                    for character in value:
                        if character == "\\":
                            esc_numbers = []

                            # esc_numbers.append(character)
                            esc_numbers.append(value[index + 1])
                            esc_numbers.append(value[index + 2])
                            esc_numbers.append(value[index + 3])

                            value = value.replace(value[index + 1], "", 1)
                            value = value.replace(value[index + 1], "", 1)
                            value = value.replace(value[index + 1], "", 1)

                            esc_sequence = int("".join(esc_numbers))
                            char = chr(esc_sequence)

                            value = value.replace(value[index], char)
                            symbol.text = value
                        index += 1
            else:
                symbol.text = ""
        elif symbol.attrib['type'] == "nil":
            if symbol.text != "nil":
                sys.stderr.write("Wrong value of type nil")
                exit(32)
        return symbol