#!/usr/bin/env python3

# IPP: 2. projekt - interpret.py
# autor: Jan Šulavík (xsulav01)


import sys

class InstructionHandler:
    def __init__(self):
        self.instruction_list = []
        self.instruction_counter = 0
        self.current_inst_number = 0
        self.label_dictionary = {}
        self.call_stack = []

    def add_to_list(self, instruction):
        self.instruction_counter += 1
        self.instruction_list.append(instruction)
        if instruction.opcode == "LABEL":
            self.add_to_labels(instruction.arg1['value'], self.instruction_counter)

    def get_instruction(self):
        if self.current_inst_number < self.instruction_counter:
            self.current_inst_number += 1
            return self.instruction_list[self.current_inst_number - 1]
        else:
            return None

    def push_to_call_stack(self, inst_number):
        self.call_stack.append(inst_number)

    def pop_from_call_stack(self):
        if len(self.call_stack) > 0:
            return self.call_stack.pop()
        else:
            sys.stderr.write("Can't pop from empty call stack!")
            exit(56)

    def add_to_labels(self, label, line):
        if label not in self.label_dictionary:
            self.label_dictionary.update({label: line})
        else:
            sys.stderr.write("Label already exists!")
            exit(52)