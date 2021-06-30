#!/usr/bin/env python3

# IPP: 2. projekt - instruction.py
# autor: Jan Šulavík (xsulav01)

import re

class Instruction:
    def __init__(self, opcode, order, arg1=None, arg2=None, arg3=None):
        self.opcode = opcode
        self.order = order
        self.arg_num = 0
        if arg1 is not None:
            self.arg_num += 1
            self.arg1 = {'type': arg1.attrib['type'], 'value': arg1.text}
        if arg2 is not None:
            self.arg_num += 1
            self.arg2 = {'type': arg2.attrib['type'], 'value': arg2.text}
        if arg3 is not None:
            self.arg_num += 1
            self.arg3 = {'type': arg3.attrib['type'], 'value': arg3.text}

    def get_symb_type(self, symb):
        symb = symb['value']

        pattern_var = re.compile(r'^(GF|LF|TF)@[A-Za-z|0-9|_|\-|$|&|%|*|!|?]+$')
        pattern_value = re.compile(r'^(bool|string|int|nil)@.*$')

        if pattern_var.search(symb) is not None:
            return "var"
        else:
            return "value"