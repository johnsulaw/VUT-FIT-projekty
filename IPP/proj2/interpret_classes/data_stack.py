#!/usr/bin/env python3

# IPP: 2. projekt - data_stack.py
# autor: Jan Šulavík (xsulav01)


import sys


class DataStack:
    def __init__(self):
        self.dataStack = []

    def push_to_data_stack(self, data):
        self.dataStack.append(data)

    def pop_from_data_stack(self):
        if len(self.dataStack) >= 1:
            return self.dataStack.pop()
        else:
            sys.stderr.write("cant pop from data stack!\n")
            exit(56)