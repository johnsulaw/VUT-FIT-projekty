#!/usr/bin/env python3

# IPP: 2. projekt - frames.py
# autor: Jan Šulavík (xsulav01)

import sys


class Frames:
    def __init__(self):
        # FRAME STACK/LOCAL FRAMES
        self.frameStack = []

        # GLOBAL FRAME
        self.globalFrame = {}

        # TEMPORARY FRAME
        self.tempFrame = {}
        self.tempFrame_defined = False

    def get_local_frame(self):
        if len(self.frameStack) >= 1:
            return self.frameStack[len(self.frameStack) - 1]
        else:
            sys.stderr.write("No local frame available!\n")
            exit(55)

    def push_to_frameStack(self, frame):
        self.frameStack.append(frame)

    def pop_from_frameStack(self):
        if len(self.frameStack) >= 1:
            return self.frameStack.pop()
        else:
            sys.stderr.write("No local frame available!\n")
            exit(55)

    def add_to_frame(self, var, frame, value=None):
        if var in frame:
            sys.stderr.write("Variable '{}' already defined in global frame!\n".format(var))
            exit(52)
        else:
            frame.update({var: value})

    def update_var_value(self, var, frame, value):
        if var in frame:
            frame.update({var: value})
        else:
            sys.stderr.write("variable {} not defined in frame {}".format(var, frame))
            exit(54)

    def declaration_check(self, var, frame):
        if var in frame:
            return 1
        else:
            sys.stderr.write("{} not declared in frame {}!\n".format(var, frame))
            exit(54)

    def get_frame(self, frame_sign):
        if frame_sign == "GF":
            return self.globalFrame
        elif frame_sign == "LF":
            return self.get_local_frame()
        elif frame_sign == "TF":
            if self.tempFrame_defined:
                return self.tempFrame
            else:
                sys.stderr.write("Temporary frame not defined!")
                exit(55)

    def create_temp_frame(self):
        self.tempFrame.clear()
        self.tempFrame_defined = True

    def get_arg_type_and_value(self, arg):
        if arg['type'] == "var":
            var_frame, var_name = self.get_variable_name_and_frame(arg)
            frame = self.get_frame(var_frame)

            if var_name not in frame:
                sys.stderr.write("Variable {} does not exist!\n".format(var_name))
                exit(54)
            else:
                value = arg['value']
                type = arg['type']

                return type, value, "var"
        elif arg['type'] in ['bool', 'string', 'int', 'type', 'label', 'nil']:
            return arg['type'], arg['value'], "value"

    def get_variable_name_and_frame(self, var):
        var_frame = var['value'].split("@")[0]
        var_name = var['value'].split("@")[1]

        return var_frame, var_name