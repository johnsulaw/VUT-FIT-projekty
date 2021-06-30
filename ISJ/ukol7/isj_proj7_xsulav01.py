#!/usr/bin/env python3

import collections

my_counter = collections.Counter()


def log_and_count(*args, **kwargs):
    """decorator factory"""
    # defined keys for decorator arguments
    key_str = "key"
    counter_str = "counts"

    def decorator(func):
        def function(*func_args, **func_kwargs):
            if key_str in kwargs:   # if 'key' is given by argument, add to 'key' counter
                kwargs[counter_str][kwargs[key_str]] += 1
            else:                   # else add to counter with function name as key
                kwargs[counter_str][func.__name__] += 1

            # print string with required information about function call
            out_string = "called {0} with {1} and {2}".format(func.__name__, func_args, func_kwargs)
            print(out_string)

            # return the called function
            return func(*func_args, **func_kwargs)
        return function
    return decorator
