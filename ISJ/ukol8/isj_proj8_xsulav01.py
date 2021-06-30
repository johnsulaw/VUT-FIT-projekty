#!/usr/bin/env python3


def first_with_given_key(iterable, key=lambda arg: arg):
    iterator = iter(iterable)
    result = []
    while True:
        value = next(iterator)
        if key(value) not in result:
            result.insert(0, key(value))
            yield value
        else:
            break
