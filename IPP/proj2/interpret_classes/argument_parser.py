#!/usr/bin/env python3

# IPP: 2. projekt - argument_parser.py
# autor: Jan Šulavík (xsulav01)

import sys

class ArgumentParser:
    def check(self):
        inputName = 0   # name of input file
        srcName = 0    # name of source file


        if len(sys.argv) != 2 and len(sys.argv) != 3:
            sys.stderr.write("Wrong number of arguments.\n")
            exit(10)
        if len(sys.argv) == 3:
            for x in range(1, 3):
                if "--input" in sys.argv[x]:
                    inpt = sys.argv[x].split("=")
                    inputName = inpt[1]
                elif "--source" in sys.argv[x]:
                    src = sys.argv[x].split("=")
                    srcName = src[1]
                else:
                    sys.stderr.write("Argument {} not supported!\n".format(sys.argv[1]))
                    exit(10)
        if len(sys.argv) == 2:
            if sys.argv[1] == "--help":
                sys.stdout.write("IPP project 2 - interpret.py\n")
            elif "--input" in sys.argv[1]:
                inpt = sys.argv[1].split("=")
                inputName = inpt[1]
                srcName = "stdin"

            elif "--source" in sys.argv[1]:
                src = sys.argv[1].split("=")
                srcName = src[1]
                inputName = "stdin"
            else:
                sys.stdout.write("Argument {} not supported!\n".format(sys.argv[1]))
                exit(10)

        return srcName, inputName